

--! patterns 的key不要出现一个key是另外的key的子串的情况
local patterns = {
	rpc_client = "fs_rpc_call",
	rpc_internal = "fs_rpc_call",
	rpc_gamed = "fs_rpc_call",
	rpc_dbd = "fs_rpc_call",
	rpc_netd = "fs_rpc_call",
}

local function translate(result, line)
	for function_prefix, efun_name in pairs(patterns) do
		--local ret = string.find(line, "rpc_client.*%(.*%).*")
		local ret = string.find(line, function_prefix..".*%(.*%).*")
		if ret then
			local declare, num = string.gsub(line, ";", "")
			assert(num == 1, "; should be the same line as function declaration")

			--local _, _, func_name = string.find(declare, "(rpc_client[^(]*)%(")
			local _, _, func_name = string.find(declare, "("..function_prefix.."[^(]*)%(")
			assert(func_name)
			func_name = string.gsub(func_name, "%s", "")

			local _, _, args = string.find(line, "%((.-)%)")
			args = args..","
			arglist = {}
			--从逗号后面往前取参数的名字
			--for v in string.gmatch(args, "%s+([^%s]-)%s*,") do
			for v in string.gmatch(args, "([_a-zA-Z0-9]-)%s*,") do
				table.insert(arglist, v)
			end

			--[[	
			if function_prefix == "rpc_gamed" then
				--insert the FS_GAMED_UID
				table.insert(arglist, 1, "FS_GAMED_UID");
			end
			]]

			-- insert the procold id, the first is vfd/uid/gamed_id .
			table.insert(arglist, 2, string.upper(func_name));



			local param = table.concat(arglist, ", ")
			--local body = "{ fs_rpc_call("..param.."); }"
			local body = "{ "..efun_name.."("..param.."); }"

			if function_prefix == "rpc_client" then
				local define = declare.." "..body
				--fin
				table.insert(result, define)
			else

				local send_declare, num = string.gsub(declare, func_name, func_name.."_send"); 
				assert(num == 1);
				local send_define = send_declare.." "..body
				table.insert(result, send_define)

				--copy the declare
				table.insert(result, declare..";")
			end

			return
		end
	end

	--没有模式，不需要处理
	table.insert(result, line)
end

function p(...)
	for i, file in ipairs(arg) do
		--print(file)
		local f = io.open(file)
		assert(f)
		local fl = {}

		--每个函数声明必须在一行内
		for line in f:lines() do
			translate(fl, line)
		end


	
		local define, _ = string.gsub(file, "_decl.c$", "")
		define, _ = string.gsub(define, "%.", "")
		define = string.gsub(define, "/", "_")
		table.insert(fl, 1, string.format("#ifndef %s\n#define %s\n", define, define))
		table.insert(fl, 2, "#include \"/rc/rpc/rpc_id.h\"\n")
		table.insert(fl, "#endif\n")

		local savefile, _ = string.gsub(file, "_decl.c$", ".h")
		sf = io.open(savefile, "w")
		local data = table.concat(fl, "\n")
		sf:write(data)
	
		f:close()
		sf:close()
	end
end

p(unpack(arg))




