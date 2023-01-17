#include "fs.hpp"

ExpressionResult checkOpenMode(std::string_view strMode, const TextRange &range, ContextPtr context, std::ios_base::openmode &mode) {
	for (const char &c : strMode) {
		switch(c) {
			case 'r':
				mode |= std::ios_base::in;
				break;
			case 'w':
				mode |= std::ios_base::out;
				break;
			case '+':
				mode |= std::ios_base::in | std::ios_base::out;
				break;
			case 'b':
				mode |= std::ios_base::binary;
				break;
			case 't':
				mode |= std::ios_base::trunc;
				if (mode & std::ios_base::ate) {
					return ExpressionResult(
						"Cannot set ate ('a') and trunc ('t') at the same time",
						range,
						context
					);
				}
				break;
			case 'a':
				mode |= std::ios_base::ate;
				if (mode & std::ios_base::trunc) {
					return ExpressionResult(
						"Cannot set ate ('a') and trunc ('t') at the same time",
						range,
						context
					);
				}
				break;
		}
	}
	return ExpressionResult();
}

ExpressionResult loader(CppModule *module) {
	module->addFunction("exists", {"filename"}, {STRING}, BOOL, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		return new Bool(
			fs::exists(args[0]->getStringValue()), range, Value::INTERPRETER
		);
	});

	module->addFunction("currentPath", {}, {}, STRING, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		std::string path = fs::current_path(ec);
		
		if (ec) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}

		return new String(path, range, Value::INTERPRETER);
	});
	
	module->addFunction("size", {"filename"}, {STRING}, INT, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		uintmax_t size = fs::file_size(args[0]->getStringValue(), ec);
		
		if (ec) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}

		return new Int(size, range, Value::INTERPRETER);
	});

	module->addFunction("createDirectory", {"dirName"}, {STRING}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		bool success = fs::create_directory(args[0]->getStringValue(), ec);
		
		if (!success) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}

		return None::empty();
	});

	module->addFunction("remove", {"filename"}, {STRING}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		bool success = fs::remove(args[0]->getStringValue(), ec);
		
		if (!success) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}

		return None::empty();
	});

	module->addFunction("rename", {"oldName", "newName"}, {STRING, STRING}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		fs::rename(args[0]->getStringValue(), args[1]->getStringValue(), ec);
		
		if (ec) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}

		return None::empty();
	});

	module->addFunction("copy", {"source", "destination"}, {STRING, STRING}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		fs::copy(
			args[0]->getStringValue(), 
			args[1]->getStringValue(), 
			fs::copy_options::update_existing | fs::copy_options::recursive, 
			ec
		);
		
		if (ec) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}

		return None::empty();
	});

	module->addFunction("copyFile", {"source", "destination"}, {STRING, STRING}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		bool success = fs::copy_file(args[0]->getStringValue(), args[1]->getStringValue(), ec);
		
		if (!success) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}

		return None::empty();
	});

	module->addFunction("isFile", {"filename"}, {STRING}, BOOL, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		bool result = fs::is_regular_file(args[0]->getStringValue(), ec);
		if (ec) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}
		return new Bool(result, range, Value::INTERPRETER);
	});

	module->addFunction("isDir", {"filename"}, {STRING}, BOOL, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		bool result = fs::is_directory(args[0]->getStringValue(), ec);
		if (ec) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}
		return new Bool(result, range, Value::INTERPRETER);
	});

	module->addFunction("isSymlink", {"filename"}, {STRING}, BOOL, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		bool result = fs::is_symlink(args[0]->getStringValue(), ec);
		if (ec) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}
		return new Bool(result, range, Value::INTERPRETER);
	});

	module->addFunction("isEmpty", {"filename"}, {STRING}, BOOL, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::error_code ec;
		bool result = fs::is_empty(args[0]->getStringValue(), ec);
		if (ec) {
			return ExpressionResult(
				ec.message(),
				range,
				context
			);
		}
		return new Bool(result, range, Value::INTERPRETER);
	});

	StructDefinition fileDefinition("File");
	fileDefinition.addMember("name", STRING);
	fileDefinition.addMember("path", STRING);
	fileDefinition.addMember("size", INT);
	fileDefinition.addMember("bin", BOOL);
	fileDefinition.addMember("writable", BOOL);
	fileDefinition.addMember("readable", BOOL);
	fileDefinition.addMember("append", BOOL);
	Struct::addStructDefinition(fileDefinition);

	module->addFunction("open", {"filename", "mode"}, {STRING, STRING}, "File", [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::ios_base::openmode mode;
		std::string modeStr = args[1]->getStringValue();
		ExpressionResult result = checkOpenMode(modeStr, range, context, mode);
		if (result.error()) {
			return result;
		}
		std::shared_ptr<std::fstream> file = std::make_shared<std::fstream>();
		file->open(args[0]->getStringValue(), mode);
		if (file->bad()) {
			return ExpressionResult(
				"Failed to open file, can't read / write",
				range,
				context
			);
		} else if (file->fail() || !file->is_open()) {
			return ExpressionResult(
				"Failed to open file",
				range,
				context
			);
		}
		Struct *fileStruct = new Struct(range, "File", Value::INTERPRETER);
		fileStruct->immutable = true;
		fs::path path = args[0]->getStringValue();
		fileStruct->setMember(
			"name", new String(path.filename().string(), range,	Value::INTERPRETER), context
		);
		fileStruct->setMember(
			"path", new String(path.parent_path().string(), range, Value::INTERPRETER), context
		);
		fileStruct->setMember(
			"size", new Int(fs::file_size(path), range, Value::INTERPRETER), context
		);
		fileStruct->setMember(
			"bin", new Bool(mode & std::ios_base::binary, range, Value::INTERPRETER), context
		);
		fileStruct->setMember(
			"writable", new Bool(mode & std::ios_base::out, range, Value::INTERPRETER), context
		);
		fileStruct->setMember(
			"readable", new Bool(mode & std::ios_base::in, range, Value::INTERPRETER), context
		);
		fileStruct->setMember(
			"append", new Bool(mode & std::ios_base::app, range, Value::INTERPRETER), context
		);
		fileStruct->setData(file);
		return fileStruct;
	});

	module->addFunction("eof", {"file"}, {"File"}, BOOL, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		return new Bool(file->eof(), range, Value::INTERPRETER);
	});

	/*
	 * READ FUNCTIONS
	*/

	module->addFunction("charCount", {"file"}, {"File"}, INT, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		// check if file is readable
		if (!static_cast<Bool*>(fileStruct->getMember("readable"))->getValue()) {
			return ExpressionResult(
				"File is not readable",
				args[0]->getRange(),
				context
			);
		}
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		int currentPos = file->tellg();
		file->seekg(0, std::ios::end);
		int size = file->tellg();
		file->seekg(currentPos, std::ios::beg);

		return new Int(size, range, Value::INTERPRETER);
	});

	module->addFunction("get", {"file"}, {"File"}, STRING, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		// check if file is readable
		if (!static_cast<Bool*>(fileStruct->getMember("readable"))->getValue()) {
			return ExpressionResult(
				"File is not readable",
				args[0]->getRange(),
				context
			);
		}
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		char c;
		if (!file->get(c))
			return String::empty();
		
		return new String(std::string(1, c), range, Value::INTERPRETER);
	});

	module->addFunction("getb", {"file"}, {"File"}, INT, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		// check if file is readable
		if (!static_cast<Bool*>(fileStruct->getMember("readable"))->getValue()) {
			return ExpressionResult(
				"File is not readable",
				args[0]->getRange(),
				context
			);
		}
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		char c;
		if (!file->get(c))
			return Int::empty();
		
		return new Int(c, range, Value::INTERPRETER);
	});

	module->addFunction("getLine", {"file"}, {"File"}, STRING, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		// check if file is readable
		if (!static_cast<Bool*>(fileStruct->getMember("readable"))->getValue()) {
			return ExpressionResult(
				"File is not readable",
				args[0]->getRange(),
				context
			);
		}
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		// check if the file is open
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		std::string line;
		if (!std::getline(*file, line)) {
			return ExpressionResult(
				"End of file",
				args[0]->getRange(),
				context
			);
		}
		return new String(line, range, Value::INTERPRETER);
	});

	module->addFunction("tellg", {"file"}, {"File"}, INT, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		return new Int(file->tellg(), range, Value::INTERPRETER);
	});

	module->addFunction("seekg", {"file", "pos", "beg"}, {"File", INT, BOOL}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		file->seekg(static_cast<Int*>(args[1])->getValue(), static_cast<Bool*>(args[2])->getValue() ? std::ios::beg : std::ios::end);
		return None::empty();
	});

	/*
	 * WRITE FUNCTIONS
	 */
	module->addFunction("write", {"file", "value"}, {"File", STRING}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		// check if file is writable
		if (!static_cast<Bool*>(fileStruct->getMember("writable"))->getValue()) {
			return ExpressionResult(
				"File is not writable",
				args[0]->getRange(),
				context
			);
		}
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		// check if the file is open
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		// write the value
		*file << args[1]->getStringValue();
		return None::empty();
	});

	module->addFunction("put", {"file", "value"}, {"File", INT}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		// check if file is writable
		if (!static_cast<Bool*>(fileStruct->getMember("writable"))->getValue()) {
			return ExpressionResult(
				"File is not writable",
				args[0]->getRange(),
				context
			);
		}
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		// check if the file is open
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		// write the value
		file->put(static_cast<Int*>(args[1])->getValue());
		return None::empty();
	});

	module->addFunction("tellp", {"file"}, {"File"}, INT, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		return new Int(file->tellp(), range, Value::INTERPRETER);
	});

	module->addFunction("seekp", {"file", "pos", "beg"}, {"File", INT, BOOL}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}

		file->seekp(static_cast<Int*>(args[1])->getValue(), static_cast<Bool*>(args[2])->getValue() ? std::ios::beg : std::ios::end);

		return None::empty();
	});

	module->addFunction("flush", {"file"}, {"File"}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		file->flush();
		return None::empty();
	});

	/*
	 * CLOSE FUNCTIONS
	 */
	
	module->addFunction("close", {"file"}, {"File"}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Struct *fileStruct = static_cast<Struct*>(args[0]);
		std::shared_ptr<std::fstream> file = static_pointer_cast<std::fstream>(fileStruct->getData());
		if (!file->is_open()) {
			return ExpressionResult(
				"File is not open",
				args[0]->getRange(),
				context
			);
		}
		file->close();
		return None::empty();
	});

	return ExpressionResult();
}
