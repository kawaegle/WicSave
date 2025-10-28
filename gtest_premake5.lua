local gtest_dir = "./lib/googletest-1.14.0/"

function check_gtest()
  if(os.isdir(gtest_dir) == false) then
    print("Gtest not found, downloading from github")
    local result_str, response_code = http.download("https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip", "gtest.zip", { sslverifypeer = 0 })
  print("Unzipping to " ..  os.getcwd() .. "/lib/")
  zip.extract("gtest.zip", os.getcwd() .. "/lib")
  os.remove("gtest.zip")
  end
end

function get_gtest_dir()
  if (os.isdir(gtest_dir)) then
    return gtest_dir
  end
  if os.isdir("../" .. gtest_dir) then
    return "../" .. gtest_dir
  end
end

function include_gtest()
  gtest_dir = get_gtest_dir()
  includedirs {
    gtest_dir .. "/googletest/include",
    gtest_dir .. "/googletest/",
  }
end

function link_gtest()
  libdirs {"./bin/%{cfg.buildcfg}/lib/"}
  links {"gtest"}
end

cppdialect "C++17"

project "gtest"
  check_gtest()
  kind "StaticLib"
  location (gtest_dir)
  language "C++"
  targetdir "bin/%{cfg.buildcfg}/lib/"

  includedirs {
    gtest_dir .. "/googletest/include",
    gtest_dir .. "/googletest"
  }
  files {
    gtest_dir .. "/googletest/src/gtest-all.cc"
  }