
-- a lua port of zfproj_creator.sh and zfproj_recursive.sh
-- usage:
--   1. zfimport or ZFLuaExecute to import this file
--      * the `private` template dir must be in the same dir of this file
--   2. functions defined in global scope:
--      * zfproj_init(ZF_TYPE[app/lib/impl], PROJ_NAME, DST_PATH)
--      * zfproj_creator(CONFIG_FILE_PATH [, DST_PATH])
--      * zfproj_recursive(SRC_DIR [, DST_DIR])
--   3. or, call this file with params to run
--      * ZFLoader zfproj.lua -app/-lib/-impl PROJ_NAME OUTPUT_PATH
--      * ZFLoader zfproj.lua CONFIG_FILE_PATH [ZF_OUTPUT]
--      * ZFLoader zfproj.lua -r SRC_DIR [DST_DIR]

local args = {...}

local function printUsage()
    ZFLogTrim('usage: (as lua functions)')
    ZFLogTrim('  zfproj_init(ZF_TYPE[app/lib/impl], PROJ_NAME, DST_PATH)')
    ZFLogTrim('  zfproj_creator(CONFIG_FILE_PATH [, DST_PATH])')
    ZFLogTrim('  zfproj_recursive(SRC_DIR [, DST_DIR])')
    ZFLogTrim('usage: (as lua cmdline)')
    ZFLogTrim('  ZFLoader zfproj.lua -app/-lib/-impl PROJ_NAME OUTPUT_PATH')
    ZFLogTrim('  ZFLoader zfproj.lua CONFIG_FILE_PATH [ZF_OUTPUT]')
    ZFLogTrim('  ZFLoader zfproj.lua -r SRC_DIR [DST_DIR]')
    ZFLogTrim('the "private" template dir must be in the same dir of zfproj.lua')
end

local appIconSetup = nil

local localPathInfo = ZFLocalPathInfo()
if zfl_eq(localPathInfo, zfnull) then
    ZFLogTrim('unable to obtain local path info')
    return
end
if not zfl_eq(localPathInfo:pathType(), ZFPathType_file()) then
    ZFLogTrim('only support from file')
    return
end
local WORK_DIR = ZFPathInfoForLocal(localPathInfo, '.'):pathData()
local ZF_ROOT_PATH = ZFPathInfoForLocal(localPathInfo, '../..'):pathData()

local _PY = nil
if os.execute('python --version >/dev/null 2>&1') then
    _PY = 'python'
elseif os.execute('python3 --version >/dev/null 2>&1') then
    _PY = 'python3'
end

function zfproj_init(ZF_TYPE, PROJ_NAME, DST_PATH)
    if PROJ_NAME == nil or PROJ_NAME == ''
        or DST_PATH == nil or DST_PATH == ''
        then
        printUsage()
        return
    end

    local CONFIG_FILE_PATH = nil
    local ZF_TYPE = nil
    local ZF_OUTPUT = nil
    local ZF_INPLACE_SRC = nil
    if zfl_eq(ZF_TYPE, 'app') then
        CONFIG_FILE_PATH = DST_PATH .. '/' .. PROJ_NAME .. '/' .. PROJ_NAME .. '/zfscript/zfautoscript_zfproj.txt'
        ZF_TYPE = 'app'
        ZF_OUTPUT = '../..'
        ZF_INPLACE_SRC = '$ZF_NAME'
    elseif ZF_TYPE == 'lib' then
        CONFIG_FILE_PATH = DST_PATH .. '/ZFFramework/ZFModule/' .. PROJ_NAME .. '/zfscript/zfautoscript_zfproj.txt'
        ZF_TYPE = 'lib'
        ZF_OUTPUT = '..'
        ZF_INPLACE_SRC = 'ZFFramework/ZFModule/$ZF_NAME'
    elseif ZF_TYPE == 'impl' then
        CONFIG_FILE_PATH = DST_PATH .. '/ZFFramework/ZFModule/' .. PROJ_NAME .. '/zfscript/zfautoscript_zfproj.txt'
        ZF_TYPE = 'impl'
        ZF_OUTPUT = '..'
        ZF_INPLACE_SRC = 'ZFFramework/ZFModule/$ZF_NAME'
    else
        printUsage()
        return
    end
    local o = ZFOutputForFile(CONFIG_FILE_PATH)
    -- ZFTAG_ADD_MODULE
    o:output('ZF_NAME = ' .. PROJ_NAME .. "\n")
    o:output('ZF_TYPE = ' .. ZF_TYPE .. "\n")
    o:output("\n")
    o:output('ZF_OUTPUT = ' .. ZF_OUTPUT .. "\n")
    o:output('ZF_INPLACE = 1' .. "\n")
    o:output('ZF_INPLACE_SRC = ' .. ZF_INPLACE_SRC .. "\n")
    o:output('# ZF_EXCLUDE +=' .. "\n")
    o:output("\n")
    o:output('ZF_APP_NAME = $ZF_NAME' .. "\n")
    o:output('ZF_APP_VERSION = 0.0.1' .. "\n")
    o:output('ZF_APP_PACKAGE = com.ZFFramework.$ZF_NAME' .. "\n")
    o:output("\n")
    o:output('ZF_LIB += ZFCore' .. "\n")
    o:output('# ZF_LIB += ZFAlgorithm' .. "\n")
    o:output('# ZF_LIB += ZFUtility' .. "\n")
    o:output('# ZF_LIB += ZFUIKit' .. "\n")
    o:output('# ZF_LIB += ZFUIWidget' .. "\n")
    o:output('# ZF_LIB += ZFUIExt' .. "\n")
    o:output('# ZF_LIB += ZFLua' .. "\n")
    o:output('# ZF_LIB += ZFUIWebKit' .. "\n")
    o:output('# ZF_LIB += ZFNet' .. "\n")
    o:output('# ZF_LIB += ZFAppUtil' .. "\n")
    o:output("\n")
    o:output('ZF_IMPL += ZF_impl' .. "\n")
    o:output('ZF_IMPL += ZFCore_impl' .. "\n")
    o:output('# ZF_IMPL += ZFAlgorithm_impl' .. "\n")
    o:output('# ZF_IMPL += ZFUIKit_impl' .. "\n")
    o:output('# ZF_IMPL += ZFLua_impl' .. "\n")
    o:output('# ZF_IMPL += ZFUIWebKit_impl' .. "\n")
    o:output('# ZF_IMPL += ZFNet_impl' .. "\n")
    o:output("\n")
    o:output('# ZF_LIB_EXT += https://github.com/ZFFramework/ZFModuleDemo_lib ZFModuleDemo_lib master' .. "\n")
    o:output("\n")
    o:output('# ZF_IMPL_EXT += https://github.com/ZFFramework/ZFModuleDemo_impl' .. "\n")
    o:output("\n")
    o:callbackClear()

    ZFLogTrim('config file created: ' .. CONFIG_FILE_PATH)
    ZFLogTrim('    use zfproj_recursive("' .. DST_PATH .. '", DST_DIR) to create entire project folder structure')
    ZFLogTrim('    or use zfproj_recursive("' .. DST_PATH .. '") to update existing proejct inplace')
end

function zfproj_creator(CONFIG_FILE_PATH, DST_PATH)
    -- formate and read config file
    if zfl_eq(CONFIG_FILE_PATH, zfnull) then
        printUsage()
        return false
    end
    local configFile = ZFInputForFile(CONFIG_FILE_PATH)
    if not configFile:valid() then
        ZFLogTrim('config file not exist: %s', CONFIG_FILE_PATH)
        return false
    end

    local config = ZFMap()
    local line = zfstring()
    local optionPattern = zfstring('^[ \\t]*(ZF_[A-Z_]+)[ \\t]*\\+?=[ \\t]*([^ \\t].*[^ \\t]|[^ \\t]+)[ \\t]*$')
    while ZFInputReadLine(line, configFile) ~= zfindexMax() do
        if ZFRegExpMatch(line, optionPattern) then
            local key = ZFRegExpReplace(line, optionPattern, '$1')
            local value = ZFRegExpReplace(line, optionPattern, '$2')
            while ZFRegExpMatch(value, '.*\\$([A-Z_]+)\\b.*') do
                local tmpKey = ZFRegExpReplace(value, '.*\\$([A-Z_]+)\\b.*', '$1')
                if zfstringIsEmpty(tmpKey) then
                    break
                end
                local tmpValue = config:get(tmpKey)
                if zfl_eq(tmpValue, zfnull) then
                    tmpValue = ''
                end
                value = ZFRegExpReplace(value, '\\$' .. tmpKey, tmpValue)
            end
            local isAdd = ZFRegExpMatch(line, '.*\\+=.*')
            if isAdd then
                local l = config:get(key)
                if zfl_eq(l, zfnull) then
                    l = ZFArray()
                    config:set(key, l)
                end
                l:add(value)
            else
                config:set(key, value)
            end
        end
        line:removeAll()
    end

    ZFLogTrim('============================================================')
    ZFLogTrim('generating project from config: ' .. CONFIG_FILE_PATH)
    ZFLogTrim('============================================================')

    -- parse config to text template params
    local param = ZFTextTemplateParam()

    if zfstringIsEmpty(config:get('ZF_NAME')) then
        ZFLogTrim('ZF_NAME not set')
        return false
    end
    param:replaceData('proj_name', config:get('ZF_NAME'))

    if not zfstringIsEmpty(DST_PATH) then
        config:set('ZF_INPLACE', '')
        config:set('ZF_INPLACE_SRC', '')
    else
        if zfstringIsEmpty(config:get('ZF_OUTPUT')) then
            ZFLogTrim('ZF_OUTPUT not set')
            return false
        end
        DST_PATH = ZFPathParentOf(CONFIG_FILE_PATH) .. '/' .. config:get('ZF_OUTPUT')
    end

    if zfstringIsEqual(config:get('ZF_TYPE'), 'app') then
        param:enableData('app_proj', zftrue)
    elseif zfstringIsEqual(config:get('ZF_TYPE'), 'lib') then
        param:enableData('lib_proj', zftrue)
    elseif zfstringIsEqual(config:get('ZF_TYPE'), 'impl') then
        param:enableData('impl_proj', zftrue)
    else
        ZFLogTrim('ZF_TYPE not set')
        return false
    end

    if zfstringIsEmpty(config:get('ZF_APP_NAME')) then
        param:replaceData('app_name', config:get('ZF_NAME'))
    else
        param:replaceData('app_name', config:get('ZF_APP_NAME'))
    end

    if zfstringIsEmpty(config:get('ZF_APP_VERSION')) then
        param:replaceData('app_version', '0.0.1')
    else
        param:replaceData('app_version', config:get('ZF_APP_VERSION'))
    end
    local ZF_APP_VERSION_CODE = zfuint(0)
    for i,e in zfl_iter(zfstringSplit(param:replaceData('app_version'), '.')) do
        local t = zfuint(e)
        ZF_APP_VERSION_CODE = ZF_APP_VERSION_CODE * 100 + t
    end
    param:replaceData('app_version_code', zfstring(ZF_APP_VERSION_CODE))

    if zfstringIsEmpty(config:get('ZF_APP_PACKAGE')) then
        param:replaceData('app_package', 'com.ZFFramework.' .. config:get('ZF_NAME'))
    else
        param:replaceData('app_package', config:get('ZF_APP_PACKAGE'))
    end

    ---@type ZFArray
    local ZF_LIB = config:get('ZF_LIB')
    if not zfl_eq(ZF_LIB, zfnull) then
        local libIndex = 0
        while libIndex < ZF_LIB:count() do
            local lib = ZF_LIB:get(libIndex)
            param:enableData('lib_require_' .. libIndex, zftrue)
            param:replaceData('lib_name_' .. libIndex, lib)
            libIndex = libIndex + 1
        end
    end
    ---@type ZFArray
    local ZF_IMPL = config:get('ZF_IMPL')
    if not zfl_eq(ZF_IMPL, zfnull) then
        local implIndex = 0
        while implIndex < ZF_IMPL:count() do
            local impl = ZF_IMPL:get(implIndex)
            param:enableData('impl_require_' .. implIndex, zftrue)
            param:replaceData('impl_name_' .. implIndex, impl)
            implIndex = implIndex + 1

            local _HAS_UIKit = (zfstringFind(impl, 'UI') ~= zfindexMax())
            if _HAS_UIKit then
                param:enableData('needUIKit', zftrue)
            end
            local _HAS_UIWebKit = (zfstringFind(impl, 'UIWeb') ~= zfindexMax())
            if _HAS_UIWebKit then
                param:enableData('needUIKit', zftrue)
                param:enableData('needUIWebKit', zftrue)
            end
            local _HAS_Net = (zfstringFind(impl, 'Net') ~= zfindexMax())
            if _HAS_Net then
                param:enableData('needNet', zftrue)
            end
        end
    end
    local function getExtName(extLine)
        local items = zfstringSplit(extLine, ' ')
        if items:count() >= 2 then
            return items:get(1)
        else
            return ZFRegExpReplace(items:get(0), '.*/([^/]+)$', '$1')
        end
    end
    ---@type ZFArray
    local ZF_LIB_EXT = config:get('ZF_LIB_EXT')
    if not zfl_eq(ZF_LIB_EXT, zfnull) then
        local libExtIndex = 0
        while libExtIndex < ZF_LIB_EXT:count() do
            local libExt = ZF_LIB_EXT:get(libExtIndex)
            param:enableData('lib_ext_require_' .. libExtIndex, zftrue)
            param:replaceData('lib_ext_git_' .. libExtIndex, libExt)
            param:replaceData('lib_ext_name_' .. libExtIndex, getExtName(libExt))
            libExtIndex = libExtIndex + 1
        end
    end
    ---@type ZFArray
    local ZF_IMPL_EXT = config:get('ZF_IMPL_EXT')
    if not zfl_eq(ZF_IMPL_EXT, zfnull) then
        local implExtIndex = 0
        while implExtIndex < ZF_IMPL_EXT:count() do
            local implExt = ZF_IMPL_EXT:get(implExtIndex)
            param:enableData('impl_ext_require_' .. implExtIndex, zftrue)
            param:replaceData('impl_ext_git_' .. implExtIndex, implExt)
            param:replaceData('impl_ext_name_' .. implExtIndex, getExtName(implExt))
            implExtIndex = implExtIndex + 1
        end
    end
    if zfstringIsEqual(config:get('ZF_TYPE'), 'impl') then
        if zfstringIsEqual(config:get('ZF_NAME'), 'ZF_impl') then
            param:enableData('needUIKit', zftrue)
        end
        local _HAS_UIKit = (zfstringFind(config:get('ZF_NAME'), 'UI') ~= zfindexMax())
        if _HAS_UIKit then
            param:enableData('needUIKit', zftrue)
        end
        local _HAS_UIWebKit = (zfstringFind(config:get('ZF_NAME'), 'UIWeb') ~= zfindexMax())
        if _HAS_UIWebKit then
            param:enableData('needUIKit', zftrue)
            param:enableData('needUIWebKit', zftrue)
        end
        local _HAS_Net = (zfstringFind(config:get('ZF_NAME'), 'Net') ~= zfindexMax())
        if _HAS_Net then
            param:enableData('needNet', zftrue)
        end
    end
    if zftrue then
        local function printState(ZF_TYPE)
            for i=0,32 do
                local require = param:enableData(ZF_TYPE .. '_require_' .. i)
                if zfl_eq(require, zfnull) or not require then
                    break
                end
                local c = param:replaceData(ZF_TYPE .. '_name_' .. i)
                local value = nil
                if zfl_eq(c, zfnull) then
                    value = ''
                else
                    value = c
                end
                ZFLogTrim('    ZFTT_C_' .. ZF_TYPE .. '_require_' .. i .. ' = %s', value)
            end
        end
        local function printEnableData(key)
            local c = param:enableData(key)
            local value = nil
            if c then
                value = '1'
            else
                value = '0'
            end
            ZFLogTrim('    ZFTT_C_%s = %s', key, value)
        end
        local function printReplaceData(key)
            local c = param:replaceData(key)
            local value = nil
            if zfl_eq(c, zfnull) then
                value = ''
            else
                value = c
            end
            ZFLogTrim('    ZFTT_R_%s = %s', key, value)
        end
        ZFLogTrim('config:')
        printEnableData('app_proj')
        printEnableData('lib_proj')
        printEnableData('impl_proj')
        printReplaceData('proj_name')
        printReplaceData('app_name')
        printReplaceData('app_version')
        printReplaceData('app_version_code')
        printReplaceData('app_package')
        printEnableData('needUIKit')
        printEnableData('needUIWebKit')
        printEnableData('needNet')
        printState('lib')
        printState('impl')
        printState('lib_ext')
        printState('impl_ext')
    end

    -- tmp dir
    ZFLogTrim('copying files...')
    local _TMP_DIR = ZF_ROOT_PATH .. '/_tmp/_zfproj_tmp_' .. config:get('ZF_NAME')
    ZFFileRemove(_TMP_DIR)
    local _TMP_DIR_SRC = nil
    local _TMP_DIR_DST = nil
    if zfstringIsEqual(config:get('ZF_TYPE'), 'app') then
        _TMP_DIR_SRC = WORK_DIR .. '/private/app/.'
        _TMP_DIR_DST = _TMP_DIR .. '/' .. param:replaceData('proj_name') .. '/'
    elseif zfstringIsEqual(config:get('ZF_TYPE'), 'lib') then
        _TMP_DIR_SRC = WORK_DIR .. '/private/lib/.'
        _TMP_DIR_DST = _TMP_DIR .. '/ZFFramework/ZFModule/' .. param:replaceData('proj_name') .. '/'
    elseif zfstringIsEqual(config:get('ZF_TYPE'), 'impl') then
        _TMP_DIR_SRC = WORK_DIR .. '/private/lib/.'
        _TMP_DIR_DST = _TMP_DIR .. '/ZFFramework/ZFModule/' .. param:replaceData('proj_name') .. '/'
    end

    local _SYNC_EXCLUDE = ZFCoreArray()
    if zfstringIsEmpty(_PY) then
        _SYNC_EXCLUDE:add('project.pbxproj')
    end
    ---@type ZFArray
    local ZF_EXCLUDE = config:get('ZF_EXCLUDE')
    if not zfl_eq(ZF_EXCLUDE, zfnull) then
        for i=0,zfl_value(ZF_EXCLUDE:count()) - 1 do
            _SYNC_EXCLUDE:add(ZF_EXCLUDE:get(i))
        end
    end
    if not zfstringIsEmpty(config:get('ZF_INPLACE')) then
        if ZFFileIsExist(DST_PATH .. '/.gitignore')
            or ZFFileIsExist(DST_PATH .. '/zfres')
            or ZFFileIsExist(DST_PATH .. '/zfsrc')
            then
            _SYNC_EXCLUDE:add('zfsrc')
            _SYNC_EXCLUDE:add('zfres')
        end
    end

    local _TMP_DIR_SRC_FORMATED = ZFPathFormat(_TMP_DIR_SRC)
    ZFIOForEach(ZFPathInfo(ZFPathType_file(), _TMP_DIR_SRC_FORMATED), function(zfargs)
        ---@type ZFIOFindData
        local fd = zfargs:sender()
        ---@type ZFPathInfo
        local pathInfo = zfargs:param0()
        local filtered = zffalse
        if fd:isDir() then
            for i=0,zfl_value(_SYNC_EXCLUDE:count()) - 1 do
                if zfstringIsEqual(fd:name(), _SYNC_EXCLUDE:get(i)) then
                    filtered = zftrue
                    pathInfo:zfv(zfnull)
                    break
                end
            end
        end
        if not filtered then
            local relPath = zfstring(pathInfo:pathData(), _TMP_DIR_SRC_FORMATED:length(), zfindexMax())
            ZFIOCopy(
                ZFPathInfo(localPathInfo:pathType(), _TMP_DIR_DST .. relPath)
                , ZFPathInfo(localPathInfo:pathType(), _TMP_DIR_SRC .. relPath)
                , zffalse
            )
        end
    end)

    -- process
    ZFTextTemplateRun(ZFPathInfo(localPathInfo:pathType(), _TMP_DIR_DST), param)

    -- xUnique if necessary
    if not zfl_eq(config:get('ZF_TYPE'), 'app') then
        if not zfstringIsEmpty(_PY) then
            ZFLogTrim('unique iOS project...')
            os.execute('sh "' .. ZF_ROOT_PATH .. '/tools/spec/iOS/unique_proj_recursive.sh" "' .. _TMP_DIR .. '"')
        end
    end

    -- appIcon
    appIconSetup(
        ZFPathInfo(localPathInfo:pathType(), _TMP_DIR_DST)
        , CONFIG_FILE_PATH
        , config
        )

    -- finally sync to dst
    ZFLogTrim('sync to target')
    local _TMP_DIR_FORMATED = ZFPathFormat(_TMP_DIR .. '/' .. config:get('ZF_INPLACE_SRC'))
    ZFIOForEach(ZFPathInfo(ZFPathType_file(), _TMP_DIR_FORMATED), function(zfargs)
        ---@type ZFIOFindData
        local fd = zfargs:sender()
        ---@type ZFPathInfo
        local pathInfo = zfargs:param0()
        local filtered = zffalse
        if fd:isDir() then
            for i=0,zfl_value(_SYNC_EXCLUDE:count()) - 1 do
                if zfstringIsEqual(fd:name(), _SYNC_EXCLUDE:get(i)) then
                    filtered = zftrue
                    pathInfo:zfv(zfnull)
                    break
                end
            end
        end
        if not filtered then
            local relPath = zfstring(pathInfo:pathData(), _TMP_DIR_FORMATED:length(), zfindexMax())
            ZFIOCopy(
                ZFPathInfo(localPathInfo:pathType(), DST_PATH .. relPath)
                , ZFPathInfo(localPathInfo:pathType(), _TMP_DIR_FORMATED .. relPath)
                , zffalse
            )
        end
    end)

    -- xUnique if necessary, with pod install
    if zfl_eq(config:get('ZF_TYPE'), 'app') and not zfstringIsEmpty(_PY) then
        ZFIOForEach(ZFPathInfo(ZFPathType_file(), _TMP_DIR_FORMATED), function(zfargs)
            ---@type ZFIOFindData
            local fd = zfargs:sender()
            ---@type ZFPathInfo
            local pathInfo = zfargs:param0()
            local filtered = zffalse
            if fd:isDir() then
                for i=0,zfl_value(_SYNC_EXCLUDE:count()) - 1 do
                    if zfstringIsEqual(fd:name(), _SYNC_EXCLUDE:get(i)) then
                        filtered = zftrue
                        pathInfo:zfv(zfnull)
                        break
                    end
                end
            end
            if not filtered then
                local relPath = zfstring(pathInfo:pathData(), _TMP_DIR_FORMATED:length(), zfindexMax())
                if zfstringEndWith(relPath, 'project.pbxproj') then
                    ZFLogTrim('unique iOS project: %s', relPath)
                    os.execute('sh "' .. ZF_ROOT_PATH .. '/tools/spec/iOS/unique_proj.sh" "' .. DST_PATH .. relPath .. '" 1')
                end
            end
        end)
    end

    ZFFileRemove(_TMP_DIR)
end

function zfproj_recursive(SRC_DIR, DST_DIR)
    local ZF_EXCLUDE_FILE = _G['ZF_EXCLUDE_FILE']
    local ZF_EXCLUDE_FILE_TMP = ZFCoreArray()
    if not zfl_eq(ZF_EXCLUDE_FILE, zfnull) then
        ZF_EXCLUDE_FILE_TMP:addFrom(ZF_EXCLUDE_FILE)
    end
    ZF_EXCLUDE_FILE_TMP:add('.git')
    ZF_EXCLUDE_FILE_TMP:add('private')
    ZF_EXCLUDE_FILE_TMP:add('zfres')
    ZF_EXCLUDE_FILE_TMP:add('_release')
    ZF_EXCLUDE_FILE_TMP:add('_repo')
    ZF_EXCLUDE_FILE_TMP:add('_tmp')

    ZF_EXCLUDE_FILE_TMP:add('.cxx')
    ZF_EXCLUDE_FILE_TMP:add('.externalNativeBuild')
    ZF_EXCLUDE_FILE_TMP:add('.gradle')
    ZF_EXCLUDE_FILE_TMP:add('.idea')
    ZF_EXCLUDE_FILE_TMP:add('build')
    ZF_EXCLUDE_FILE_TMP:add('out')

    local SRC_DIR_FORMATED = ZFPathFormat(SRC_DIR)
    local DST_DIR_FORMATED = ZFPathFormat(DST_DIR)
    ZFIOForEach(ZFPathInfo(ZFPathType_file(), SRC_DIR_FORMATED), function(zfargs)
        ---@type ZFIOFindData
        local fd = zfargs:sender()
        ---@type ZFPathInfo
        local pathInfo = zfargs:param0()
        local filtered = (not zfstringIsEqual(fd:name(), 'zfautoscript_zfproj.txt'))
        if fd:isDir() then
            for i=0,zfl_value(ZF_EXCLUDE_FILE_TMP:count()) - 1 do
                if zfstringIsEqual(fd:name(), ZF_EXCLUDE_FILE_TMP:get(i)) then
                    filtered = zftrue
                    pathInfo:zfv(zfnull)
                    break
                end
            end
        else
            filtered = (not zfstringIsEqual(fd:name(), 'zfautoscript_zfproj.txt'))
        end
        if not filtered then
            zfproj_creator(pathInfo:pathData(), DST_DIR_FORMATED)
        end
    end)
end

local function zfproj_entry()
    if #args > 0 then
        if ZFRegExpMatch(args[1], '^(-app|-lib|-impl)$') then
            if #args < 3 then
                printUsage()
                return
            end
            zfproj_init(args[1], args[2], args[3])
        elseif ZFRegExpMatch(args[1], '^(-r)$') then
            if #args < 2 then
                printUsage()
                return
            end
            zfproj_recursive(args[2], args[3])
        else
            zfproj_creator(args[1], args[2])
        end
        ZFApp.appExit(0)
    end
end

appIconSetup = function(dstPath, CONFIG_FILE_PATH, config)
    local ZF_NAME = config:get('ZF_NAME')
    if zfl_eq(ZF_NAME, zfnull) then
        return
    end
    if not zfl_eq(config:get('ZF_TYPE'), 'app') then
        return
    end

    local iconPathInfo = ZFPathInfoForLocal(ZFPathInfo(ZFPathType_file(), CONFIG_FILE_PATH), 'AppIcon.png')
    if not ZFIOIsExist(iconPathInfo) then
        iconPathInfo = ZFPathInfoForLocal(ZFPathInfo(ZFPathType_file(), ZF_ROOT_PATH), 'master/tools/AppIcon.png')
    end
    if not ZFIOIsExist(iconPathInfo) then
        return
    end
    local icon = ZFUIImageFromInput(ZFInputForPathInfo(iconPathInfo))
    if zfl_eq(icon, zfnull) then
        return
    end
    ZFLogTrim('generating icon from: %s', iconPathInfo)

    local iconSize = function(size)
        return ZFUISizeCreate(size / 2)
    end
    local iconConv = function(relPath, size)
        local output = ZFOutputForPathInfo(ZFPathInfoForLocal(dstPath, relPath))
        ZFUIImageToOutput(output, ZFUIImageScale(icon, iconSize(size)))
        output:ioClose()
    end

    iconConv(zfstr('%s/zfproj/Android/%s/zfapp/src/main/res/mipmap-xxxhdpi/ic_launcher.png', ZF_NAME, ZF_NAME), 192)
    iconConv(zfstr('%s/zfproj_with_src/Android/%s/zfapp/src/main/res/mipmap-xxxhdpi/ic_launcher.png', ZF_NAME, ZF_NAME), 192)

    iconConv(zfstr('%s/zfproj/cmake/%s/AppIcon.png', ZF_NAME, ZF_NAME), 192)
    iconConv(zfstr('%s/zfproj_with_src/cmake/%s/AppIcon.png', ZF_NAME, ZF_NAME), 192)

    iconConv(zfstr('%s/zfproj/iOS/%s/ProjectFiles/Assets.xcassets/AppIcon.appiconset/AppIcon_256.png', ZF_NAME, ZF_NAME), 256)
    iconConv(zfstr('%s/zfproj/iOS/%s/ProjectFiles/Assets.xcassets/AppIcon.appiconset/AppIcon_1024.png', ZF_NAME, ZF_NAME), 1024)
    iconConv(zfstr('%s/zfproj_with_src/iOS/%s/ProjectFiles/Assets.xcassets/AppIcon.appiconset/AppIcon_256.png', ZF_NAME, ZF_NAME), 256)
    iconConv(zfstr('%s/zfproj_with_src/iOS/%s/ProjectFiles/Assets.xcassets/AppIcon.appiconset/AppIcon_1024.png', ZF_NAME, ZF_NAME), 1024)
end

zfproj_entry()

