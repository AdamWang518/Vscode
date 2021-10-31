string num_str1(std::to_string(intValue));
    char *cstr =  const_cast<char*>(num_str1.c_str());
	f.Write(cstr, strlen(cstr));
	f.Write("\n", 1);