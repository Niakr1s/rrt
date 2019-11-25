from pathlib import Path, PurePath
import os
import shutil
import zipfile
import sys

PROJECT_NAME = "rrt"
BIN_DIR = "release"
BIN_BIN_DIR = BIN_DIR + "/bin"
RELEASE_DIR = BIN_DIR + "/release"

CMAKE_PRE_BUILD_CMD = "cmake . -B" + BIN_DIR
CMAKE_BUILD_CMD = "cmake --build " + BIN_DIR + " --config MinSizeRel -j4"

BOOST_VERSION = "1_71"
BOOST_PREFIX = "libboost_"
BOOST_SUFFIX = "-mgw73-mt-x64-" + BOOST_VERSION + ".dll"

BOOST_LIBS = ["date_time", "filesystem",
              "thread", "log"]  # change it to include in zip

IGNORED_FILES = ["D3Dcompiler_47.dll",
                 "opengl32sw.dll", "libEGL.dll", "libGLESV2.dll"]  # these files will be deleted from release


def cmake(clean=False):
    if clean:
        if Path(BIN_DIR).exists():
            shutil.rmtree(BIN_DIR)
    os.system(CMAKE_PRE_BUILD_CMD)
    os.system(CMAKE_BUILD_CMD)


def findFile(src, filename):
    for dirpath, _, filenames in os.walk(src):
        for file in filenames:
            if file.lower() == filename:
                print("Found " + filename + " in " + dirpath)
                return dirpath + "\\" + file
    print(filename + " not found")
    quit()


def addToZip(version=""):
    zipPath = BIN_DIR + "/" + PROJECT_NAME + " " + version + ".zip"
    if Path(zipPath).exists():
        os.remove(zipPath)
    with zipfile.ZipFile(zipPath, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=9) as releaseZip:
        for dirpath, _, filenames in os.walk(RELEASE_DIR):
            for filename in filenames:
                fullPath = dirpath + "/" + filename
                fullPathWithoutReleaseDir = fullPath.replace(RELEASE_DIR, "")
                if filename not in IGNORED_FILES:
                    releaseZip.write(dirpath + "/" + filename,
                                     fullPathWithoutReleaseDir)
        for lib in BOOST_LIBS:
            fullName = BOOST_PREFIX + lib + BOOST_SUFFIX
            releaseZip.write(findFile("c:/boost/lib", fullName), fullName)
    print('Zip created in "' + zipPath + '"')


def prepareReleaseDir():
    if Path(RELEASE_DIR).exists():
        shutil.rmtree(RELEASE_DIR)
    shutil.copytree(BIN_BIN_DIR, RELEASE_DIR)
    print("Release dir " + RELEASE_DIR + " prepared")


def getProjectVersion():
    versionPath = findFile(BIN_DIR, "version.h")
    if versionPath == "":
        return
    with open(versionPath) as f:
        for line in f:
            if (line.find("PROJECT_VER") != -1):
                projectVersion = line.split('"')[1]
                print("Got Project Version = " + projectVersion)
                return projectVersion
    return


if __name__ == "__main__":
    if (len(sys.argv) == 0):
        cmake(clean=False)
    elif (sys.argv[0] == "--clean"):
        cmake(clean=True)

    prepareReleaseDir()
    os.system(findFile(os.getenv("QT_DIR", "c:/Qt"),
                       "windeployqt.exe") + " --release " + RELEASE_DIR)
    addToZip(getProjectVersion())
