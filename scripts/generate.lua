--
-- Embed the GraphQL/REST queries into src/requests/<api>.c
-- - variables-structures (xx_vars_t)
-- - snprintf-style functions (xx_snprintVars(char* dst, size_t n, xx_vars_tt))
-- -
-- This script generates both the header/source files.
--

local script_path = path.getdirectory(_SCRIPT)
local source_path = path.join(script_path, "..", "src")

function string.symbolicate(self)
	return self:gsub("[/%.]", "_")
end

function string.prefix_camelCase(self)
	local p = self:explode('_')
	local r = p[1] .. "_" .. p[2]
	for i=3,#p,1 do
		r = r .. p[i]:gsub("(%w)(%w+)", function(a, b) return a:upper() .. b end)
	end
	return r
end

function string.prefix_PascalCase(self)
	local p = self:explode('_')
	local r = p[1] .. "_"
	for i=2,#p,1 do
		r = r .. p[i]:gsub("(%w)(%w+)", function(a, b) return a:upper() .. b end)
	end
	return r
end

function string.camelCase(self)
	local p = self:explode('_')
	local r = p[1]
	for i=2,#p,1 do
		r = r .. p[i]:gsub("(%w)(%w+)", function(a, b) return a:upper() .. b end)
	end
	return r
end

function string.PascalCase(self)
	local p = self:explode('_')
	local r = ""
	for i=1,#p,1 do
		r = r .. p[i]:gsub("(%w)(%w+)", function(a, b) return a:upper() .. b end)
	end
	return r
end

function toCtype(type)
	return type:gsub("(%w+)", {
		["String"] = "const char*",
		["ID"] = "const char*",
		["URI"] = "const char*",
		["RepositoryVisibility"] = "const char*", --github
		["VisibilityLevelsEnum"] = "const char*", --gitlab/GraphQL
		["Int"] = "int",
		["Bool"] = "bool",
	})
end

function toPrintf(type)
	return type:gsub("(%w+)", {
		["String"] = '\\"%s\\"',
		["ID"] = '\\"%s\\"',
		["URI"] = '\\"%s\\"',
		["RepositoryVisibility"] = '\\"%s\\"', --github
		["VisibilityLevelsEnum"] = '\\"%s\\"', --gitlab
		["Int"] = '%i',
		["Bool"] = '\\"%s\\"',
	})
end

function toPrintfReplace(type)
	return type:gsub("(%w+)", {
		["String"] = '%s',
		["ID"] = '%s',
		["URI"] = '%s',
		["RepositoryVisibility"] = '%s', --github
		["VisibilityLevelsEnum"] = '%s', --gitlab
		["Int"] = '%i',
		["Bool"] = '%s',
	})
end

function writeStruct(align, name, vars)
	assert(name)
	assert(vars)

	_p(align, "typedef struct %s", name)
	_p(align, "{")
	for v,t in table.sortedpairs(vars) do
		_p(align+1, "%s %s;", toCtype(t),v)
	end
	_p(align, "}")
	_p(align, "%s;", name)
	_p(align, "")
end

function table.index(t,item)
	for i,e in ipairs(t) do
		if e == item then return i end
	end
	return nil
end

function table.sortednext(t,cur)
	local keys = table.keys(t)
	table.sort(keys)
	local i = table.index(keys,cur)
	return i and keys[i+1] or nil
end

function writeVariablesString(align, name, vars)
	assert(vars)

	if name then
		_p(align, '"\\"%s\\": {"', name)
	else
		_p(align, '"{"')
	end

	for v,t in table.sortedpairs(vars) do
		_p(align, '" \\"_%s\\": %s%s"', v, toPrintf(t), table.sortednext(vars,v) and ',' or '')
	end
	_p(align, '"}"')
end

function writeFunction(align, name, ret, ...)
	assert(name)
	local params = {...}

	_p(align, "%s %s(", ret, name)
	for i,p in ipairs(params) do
		_p(align+1, "%s%s", p, next(params,i) and ',' or '')
	end
	_p(align, ")")
end

function writeFunctionDeclaration(align, name, ret, ...)
	writeFunction(align, name, ret, ...)
	_p(align, ";")
	_p(align, "")
end

function writeFunction_snprintJsonVars(align, name, ret, buf, count, varsData, vars)
	local varsName = nil
	if varsData then
		varsName = string.explode(varsData, ' ')
		varsName = varsName[#varsName]
	end

	writeFunction(align, name, ret, buf, count, varsData)
	_p(align, "{")
	--_p(align, 1, "assert(%s);", varsName)
	-- _p(align, "")
	_p(align+1, "return stbsp_snprintf(")
	_p(align+2, "%s,", buf:gsub('char%*%s*', ''))
	_p(align+2, "%s,", count:gsub('int%s*', ''))

	if vars and next(vars) then
		writeVariablesString(align+2, nil, vars)
		for v,t in table.sortedpairs(vars) do
			_p(align+2, ", %s->%s", varsName, v)
			if t == "Bool" then
				_p(align+3, "? \"true\" : \"false\"")
			end
		end
	end
	_p(align+1, ");")
	_p(align, "}")
	_p(align, "")
end


function writeFunction_snprintJsonRequest(align, name, ret, buf, count, symbol, varsData, vars)
	local varsName = nil
	if varsData then
		varsName = string.explode(varsData, ' ')
		varsName = varsName[#varsName]
	end

	writeFunction(align, name, ret, buf, count, varsData)
	_p(align, "{")
	--_p(align+1, "assert(%s);", varsName)
	--_p(align, "")

	_p(align+1, "return stbsp_snprintf(")
	_p(align+2, "%s,", buf:gsub('char%*%s*', ''))
	_p(align+2, "%s,", count:gsub('int%s*', ''))
	_p(align+2, '"{"')

	if vars then
		writeVariablesString(align+2, "variables", vars)
		_p(align+2, '","')
	end

	_p(align+2, '"\\"query\\": \\"%s\\""', [[%s]])
	_p(align+2, '"} \\n"')

	if vars then
		for v,t in table.sortedpairs(vars) do
			_p(align+2, ",%s->%s", varsName, v)
			if t == "Bool" then
				_p(align+3, "? \"true\" : \"false\"")
			end
		end
	end

	_p(align+2, ",g_%s_de", symbol)
	_p(align+1, ");")
	_p(align, "}")
	_p(align, "")
end


function writeFunction_snprintUrl(align, name, ret, buf, count, endpointUrl, route, varsData, vars)
	local varsName = nil
	if varsData then
		varsName = string.explode(varsData, ' ')
		varsName = varsName[#varsName]
	end

	writeFunction(align, name, ret, buf, count, endpointUrl, varsData)
	_p(align, "{")
	--_p(align+1, "assert(%s);", varsName)
	--_p(align, "")

	_p(align+1, "return stbsp_snprintf(")
	_p(align+2, "%s,", buf:gsub('char%*%s*', ''))
	_p(align+2, "%s,", count:gsub('int%s*', ''))
	_p(align+2, '"%s/%s"', "%s", route:gsub("%$_%w+%s*:%s*(%w+)", toPrintfReplace):gsub('\n', ''))

	_p(align+2, ", %s", endpointUrl:gsub('const%schar%*%s*', ''))

	if vars then
		for vv in route:gmatch("%$_(%w+)%s*:%s*%w+") do
			_p(align+2, ",%s->%s", varsName, vv)
		end
	end

	_p(align+1, ");")
	_p(align, "}")
	_p(align, "")
end

function writeFunction_snprintRestJson(align, name, ret, buf, count, varsData, queryJson, vars)
	local varsName = nil
	if varsData then
		varsName = string.explode(varsData, ' ')
		varsName = varsName[#varsName]
	end

	writeFunction(align, name, ret, buf, count, varsData)
	_p(align, "{")
	--_p(align+1, "assert(%s);", varsName)
	--_p(align, "")

	_p(align+1, "return stbsp_snprintf(")
	_p(align+2, "%s,", buf:gsub('char%*%s*', ''))
	_p(align+2, "%s,", count:gsub('int%s*', ''))

	_p(align+2, '"%s"', queryJson:gsub("%$_%w+%s*:%s*(%w+)", toPrintfReplace):gsub('"', '\\"'):gsub('\n', '"\n"'))

	--_p(align+2, ", %s", endpointUrl:gsub('char%*%s*', ''))

	if vars then
		for vv in queryJson:gmatch("%$_(%w+)%s*:%s*%w+") do
			_p(align+2, ",%s->%s", varsName, vv)
		end
	end

	_p(align+1, ");")
	_p(align, "}")
	_p(align, "")
end


function generate_graphql(files, queries_c, header)
	local outpath = queries_c
	queries_c = path.getrelative(source_path, queries_c)
	print("generating", queries_c)

	-- open queries.c and write the file header
	io.capture()
	local out = io.open(outpath, "w")
	local define = "GEARS_" .. string.symbolicate(queries_c):upper() .. "_INC"
	_p("//! git-gears-c's GraphQL queries")
	_p("//! as structures and snprintf-style functions")
	_p("//! DO NOT EDIT - this file is autogenerated.")
	_p("//! Run `genie generate` or `make generate` to re-generate.")
	_p("")

	if header then
		_p("#ifndef %s", define)
		_p("#define %s", define)
		_p("")
		_p("#include <%s>", "stdbool.h")
		_p("")
	else
		_p("#include %q", "stb_printf.h")
		_p("#include %q", path.getbasename(queries_c) .. ".h")
		_p("#include %q", "github_graphql.h") --TODO: select which one
		_p("#include %q", "gitlab_graphql.h")
		_p("")
	end

	for i,gql in ipairs(files) do
		local gql_relative = path.getrelative(source_path, gql)
		local gql_symbol = string.symbolicate(path:getrelative(path.getdirectory(outpath), gql))
		print(gql, gql_symbol, gql_relative)
		local f = io.open(gql, "r")
		assert(f)
		local s = f:read("*all")
		local vars = {}
		s:gsub("%$_(%w+)%s*:%s*(%w+)", function(var, type) vars[var]=type end)
		printtable('vars', vars)

		print(gql_symbol, string.prefix_camelCase(gql_symbol))
		print(gql_symbol, string.camelCase(gql_symbol))
		print(gql_symbol, string.prefix_PascalCase(gql_symbol))
		print(gql_symbol, string.PascalCase(gql_symbol))

		if header then
			if next(vars) then
				_p("//! query data")
				writeStruct(0, string.PascalCase(gql_symbol), vars)
				_p("//! snprint variables JSON")
				writeFunctionDeclaration(0, "snprintJsonVars_"..string.PascalCase(gql_symbol), "int", "char* buf", "int count", "const " .. string.PascalCase(gql_symbol).."* vars")
				_p("//! snprint request JSON")
				writeFunctionDeclaration(0, "snprintJsonRequest_"..string.PascalCase(gql_symbol), "int", "char* buf", "int count", "const " .. string.PascalCase(gql_symbol).."* vars")
			else
				_p("//! snprint request JSON")
				writeFunctionDeclaration(0, "snprintJsonRequest_"..string.PascalCase(gql_symbol), "int", "char* buf", "int count")
			end
		else
			if next(vars) then
				writeFunction_snprintJsonVars(0, "snprintJsonVars_"..string.PascalCase(gql_symbol), "int", "char* buf", "int count", "const " .. string.PascalCase(gql_symbol).."* vars", vars)
				writeFunction_snprintJsonRequest(0, "snprintJsonRequest_"..string.PascalCase(gql_symbol), "int", "char* buf", "int count", gql_symbol, "const " .. string.PascalCase(gql_symbol).."* vars", vars)
			else
				writeFunction_snprintJsonRequest(0, "snprintJsonRequest_"..string.PascalCase(gql_symbol), "int", "char* buf", "int count", gql_symbol)
			end
		end
		f:close()
	end

	if header then
		_p("\n#endif // %s", define)
	end

	out:write(io.endcapture())
	out:close()
	io.flush()
	os.executef("clang-format -i %s", outpath)
end

function generate_rest(files, queries_c, header)
	local outpath = queries_c
	queries_c = path.getrelative(source_path, queries_c)
	print("generating", queries_c)

	-- open queries.c and write the file header
	io.capture()
	local out = io.open(outpath, "w+b")
	local define = "GEARS_" .. queries_c:symbolicate():upper() .. "_INC"
	_p("//! git-gears-c's REST queries")
	_p("//! as structures and snprintf-style functions")
	_p("//! DO NOT EDIT - this file is autogenerated.")
	_p("//! Run `genie generate` or `make generate` to re-generate.")
	_p("")

	if header then
		_p("#ifndef %s", define)
		_p("#define %s", define)
		_p("")
		_p("#include <%s>", "stdbool.h")
		_p("")
	else
		_p("#include %q", "stb_printf.h")
		_p("#include %q", path.getbasename(queries_c) .. ".h")
		_p("")
	end

	for i,route in ipairs(files) do
		local route_relative = path.getrelative(source_path, route)
		local route_symbol = path:getrelative(path.getdirectory(outpath), route):symbolicate()
		print(route, route_symbol, route_relative)
		local f = io.open(route, "r")
		assert(f)
		local s = f:read("*all")
		local routevars = {}
		s:gsub("%$_(%w+)%s*:%s*(%w+)", function(var, type) routevars[var]=type end)
		printtable('routevars', routevars)
		f:close()


		if header then
			if next(routevars) then
				_p("//! url data")
				writeStruct(0, string.PascalCase(route_symbol), routevars)
				_p("//! snprint url")
				writeFunctionDeclaration(0, "snprintUrl_"..string.PascalCase(route_symbol), "int", "char* buf", "int count", "const char* endpointUrl", "const " .. string.PascalCase(route_symbol).."* vars")
			else
				_p("//! snprint url")
				writeFunctionDeclaration(0, "snprintUrl_"..string.PascalCase(route_symbol), "int", "char* buf", "int count", "const char* endpointUrl")
			end
		else
			if next(routevars) then
				writeFunction_snprintUrl(0, "snprintUrl_"..string.PascalCase(route_symbol), "int", "char* buf", "int count", "const char* endpointUrl", s, "const " .. string.PascalCase(route_symbol).."* vars", routevars)
			else
				writeFunction_snprintUrl(0, "snprintUrl_"..string.PascalCase(route_symbol), "int", "char* buf", "int count", "const char* endpointUrl", s)
			end
		end


		local rest = path.replaceextension(route, ".json")
		local rest_relative = path.getrelative(source_path, rest)
		local rest_symbol = path:getrelative(path.getdirectory(outpath), rest):symbolicate()
		print(rest, rest_symbol, rest_relative)


		local jsonvars = {}
		if os.isfile(rest) then
			f = io.open(rest, "r")
			assert(f)
			s = f:read("*all")
			s:gsub("%$_(%w+)%s*:%s*(%w+)", function(var, type) jsonvars[var]=type end)
			printtable('jsonvars', jsonvars)
			f:close()
		end

		if next(jsonvars) then
			if header then
				_p("//! rest data")
				writeStruct(0, string.PascalCase(rest_symbol), jsonvars)
				_p("//! snprint rest JSON")
				writeFunctionDeclaration(0, "snprintRestJson_"..string.PascalCase(rest_symbol), "int", "char* buf", "int count", "const " .. string.PascalCase(rest_symbol).."* vars")
			else
				writeFunction_snprintRestJson(0, "snprintRestJson_"..string.PascalCase(rest_symbol), "int", "char* buf", "int count", "const " .. string.PascalCase(rest_symbol).."* vars", s, jsonvars)
			end
		end
	end

	if header then
		_p("\n#endif // %s", define)
	end

	out:write(io.endcapture())
	out:close()
	io.flush()
	os.executef("clang-format -i %s", outpath)
end

function dogenerate()
	local githubfiles = os.matchfiles(path.join(script_path, "..", "src", "requests", "github", "*.graphql"))
	generate_graphql(githubfiles, path.join(script_path,"..", "src", "requests", "github_graphqlutils.h"), true)
	generate_graphql(githubfiles, path.join(script_path,"..", "src", "requests", "github_graphqlutils.c"))

	local gitlabfiles = os.matchfiles(path.join(script_path, "..", "src", "requests", "gitlab", "*.graphql"))
	generate_graphql(gitlabfiles, path.join(script_path,"..", "src", "requests", "gitlab_graphqlutils.h"), true)
	generate_graphql(gitlabfiles, path.join(script_path,"..", "src", "requests", "gitlab_graphqlutils.c"))

	githubfiles = os.matchfiles(path.join(script_path, "..", "src", "requests", "github", "*.route"))
	generate_rest(githubfiles, path.join(script_path,"..", "src", "requests", "github_restutils.h"), true)
	generate_rest(githubfiles, path.join(script_path,"..", "src", "requests", "github_restutils.c"))

	gitlabfiles = os.matchfiles(path.join(script_path, "..", "src", "requests", "gitlab", "*.route"))
	generate_rest(gitlabfiles, path.join(script_path,"..", "src", "requests", "gitlab_restutils.h"), true)
	generate_rest(gitlabfiles, path.join(script_path,"..", "src", "requests", "gitlab_restutils.c"))
end
