// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "FileSystem/FileSystem.h"

#include "Platform/Macros.h"
#if HAS_FILESYSTEM
#    include <experimental/filesystem>
namespace stdfs = std::experimental::filesystem;
#else
#    include <climits>
#    include <cstdlib>
#    include <sys/stat.h>
#    include <unistd.h>

#    include "Common/Logging.h"
#endif

#if defined(RAINBOW_OS_WINDOWS)
#    define kPathSeparator "\\"
#else
#    define kPathSeparator "/"
#    if defined(RAINBOW_OS_ANDROID)
#        include <android/native_activity.h>
extern ANativeActivity* g_native_activity;
#    elif defined(RAINBOW_OS_IOS)
#        include "Platform/iOS/NSString+Rainbow.h"
#    endif
#endif
#include "FileSystem/Bundle.h"

namespace
{
#ifndef RAINBOW_OS_ANDROID
    constexpr char kUserDataPath[] = kPathSeparator "user";
#endif

    const rainbow::Bundle* g_bundle{};
}  // namespace

auto rainbow::filesystem::absolute(czstring path) -> Path
{
    return Path{path};
}

auto rainbow::filesystem::assets_path() -> czstring
{
    return g_bundle == nullptr ? nullptr : g_bundle->assets_path();
}

bool rainbow::filesystem::create_directories(czstring path,
                                             std::error_code& error)
{
#if HAS_FILESYSTEM
    return stdfs::create_directories(path, error);
#else
    bool end = false;
    struct stat sb;
    char dir[PATH_MAX];
    strncpy(dir, path, PATH_MAX);
    for (int i = 1; !end; ++i)
    {
        end = dir[i] == '\0';
        if (dir[i] == '/' || end)
        {
            dir[i] = '\0';
            if (stat(dir, &sb) != 0)
            {
                if (errno != ENOENT)
                {
                    error = std::error_code(errno, std::generic_category());
                    LOGE("Error accessing '%s' (%x)", dir, errno);
                    return false;
                }
                if (mkdir(dir, 0775) != 0 || stat(dir, &sb) != 0)
                {
                    error = std::error_code(errno, std::generic_category());
                    LOGE("Failed to create directory '%s' (%x)", dir, errno);
                    return false;
                }
            }
            if (!S_ISDIR(sb.st_mode))
            {
                error = std::make_error_code(std::errc::not_a_directory);
                LOGE("'%s' is not a valid directory", dir);
                return false;
            }
            dir[i] = '/';
        }
    }
    return true;
#endif
}

auto rainbow::filesystem::executable_path() -> czstring
{
    return g_bundle == nullptr ? nullptr : g_bundle->exec_path();
}

bool rainbow::filesystem::exists(czstring p)
{
    auto path = relative(p);
#if defined(RAINBOW_OS_ANDROID)
    auto asset = AAssetManager_open(
        g_native_activity->assetManager, path.c_str(), AASSET_MODE_UNKNOWN);
    AAsset_close(asset);
    return asset != nullptr;
#elif HAS_FILESYSTEM
    std::error_code error;
    return stdfs::exists(path.c_str(), error);
#else
    struct stat sb;
    return stat(path.c_str(), &sb) == 0;
#endif
}

void rainbow::filesystem::initialize(const Bundle& bundle)
{
    g_bundle = &bundle;
}

bool rainbow::filesystem::is_directory(czstring path,
                                       [[maybe_unused]] std::error_code& error)
{
#if HAS_FILESYSTEM
    return stdfs::is_directory(path, error);
#else
    struct stat sb;
    return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
#endif
}

bool rainbow::filesystem::is_regular_file(
    czstring path, [[maybe_unused]] std::error_code& error)
{
#if HAS_FILESYSTEM
    return stdfs::is_regular_file(path, error);
#else
    struct stat sb;
    return stat(path, &sb) == 0 && S_ISREG(sb.st_mode);
#endif
}

auto rainbow::filesystem::main_script() -> czstring
{
    return g_bundle == nullptr ? nullptr : g_bundle->main_script();
}

auto rainbow::filesystem::path_separator() -> czstring
{
    return kPathSeparator;
}

auto rainbow::filesystem::relative(czstring path) -> Path
{
#if defined(RAINBOW_OS_ANDROID)
    Path relative_path;
    // Android doesn't ignore multiple '/' in paths.
    for (int i = 0; path[i] != '\0'; ++i)
    {
        if (path[i] == '/' && path[i + 1] == '/')
            continue;
        relative_path += path[i];
    }
    return relative_path;
#elif defined(RAINBOW_OS_IOS)
    NSString* str = [NSString stringWithUTF8StringNoCopy:path];
    str = [NSBundle.mainBundle pathForResource:str.stringByDeletingPathExtension
                                        ofType:str.pathExtension];
    return str == nil ? Path{} : Path{str.UTF8String};
#else
    Path relative_path{assets_path()};
    relative_path /= path;
    return relative_path;
#endif
}

bool rainbow::filesystem::remove(czstring path,
                                 [[maybe_unused]] std::error_code& error)
{
#if HAS_FILESYSTEM
    return stdfs::remove(path, error);
#else
    return ::remove(path) == 0;
#endif
}

auto rainbow::filesystem::system_current_path() -> std::string
{
#if HAS_FILESYSTEM
    return stdfs::current_path().u8string();
#else
    char cwd[PATH_MAX];
    zstring result = getcwd(cwd, PATH_MAX);

    R_ASSERT(result == cwd, "Failed to get current working directory");

    return result;
#endif
}

auto rainbow::filesystem::user(czstring path) -> Path
{
#ifdef RAINBOW_OS_IOS
    NSError* err = nil;
    auto library_dir = [[NSFileManager.defaultManager  //
          URLForDirectory:NSLibraryDirectory
                 inDomain:NSUserDomainMask
        appropriateForURL:nil
                   create:YES
                    error:&err] path];
    if (library_dir == nil)
        return {};

    NSString* user_path = [NSString stringWithUTF8StringNoCopy:path];
    user_path = [library_dir stringByAppendingPathComponent:user_path];
    return Path{user_path.UTF8String};
#else
    auto data_path = user_data_path();
    std::error_code error;
    if (data_path == nullptr || (!is_directory(data_path, error) &&
                                 (!create_directories(data_path, error) ||
                                  !is_directory(data_path, error))))
    {
        return {};
    }

    Path user_path{user_data_path()};
    user_path /= path;
    return user_path;
#endif  // RAINBOW_OS_IOS
}

auto rainbow::filesystem::user_data_path() -> czstring
{
    static std::string data_path;
    if (data_path.empty())
    {
#ifdef RAINBOW_OS_ANDROID
        czstring path = g_native_activity->externalDataPath;
        if (path == nullptr)
            path = g_native_activity->internalDataPath;
        if (path != nullptr)
            data_path = path;
#else
        data_path = assets_path();
        data_path += kUserDataPath;
#endif
    }

    return data_path.c_str();
}

#ifdef RAINBOW_TEST
namespace rainbow::filesystem::test
{
    auto bundle() -> const Bundle* { return g_bundle; }
}  // namespace rainbow::filesystem::test
#endif
