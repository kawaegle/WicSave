local wicsave_dir="./"

function check_wicsave()
  if(os.isdir(wicsave_dir) == false) then
    print("save not found, downloading from github")
    local result_str, response_code = http.download("https://github.com/kawaegle/safeSave/archive/refs/heads/master.zip", "WicSave.zip", { sslverifypeer = 0 })
  print("Unzipping to " ..  os.getcwd() .. "/lib")
  zip.extract("WicSave.zip", os.getcwd() .. "/lib")
  os.remove("WicSave.zip")
  os.rename("./lib/safeSave-master/", "./lib/safeSave/")
  end
end

function get_WicSave_dir()
  if (os.isdir(wicsave_dir)) then
    return wicsave_dir
  end
  if os.isdir("../" .. wicsave_dir) then
    return "../" .. wicsave_dir
  end
end

function include_wicsave()
  includedirs { wicsave_dir .. "/include/" }
end

function link_wicsave()
  libdirs { "bin/%{cfg.buildcfg}/lib/" }
  links { "WicSave" }
end

project ("WicSave")
  check_wicsave()
  kind "StaticLib"
  location (save_dir)
  language "C++"
  defines { "_X86_" }
  targetdir "bin/%{cfg.buildcfg}/lib/"

  include_wicsave()
  files {
    wicsave_dir .. "/src/*.cpp"
  }