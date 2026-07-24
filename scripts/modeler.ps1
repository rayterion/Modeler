[CmdletBinding()]
param(
	[Parameter(Position = 0)]
	[ValidateSet("setup", "build", "test", "run", "docs", "clean", "help")]
	[string]$Command = "help",

	[ValidateSet("Debug", "Release")]
	[string]$Configuration = "Debug",

	[switch]$SystemDeps
)

$ErrorActionPreference = "Stop"

$RootDir = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$BuildDir = Join-Path $RootDir "build"

function Invoke-External {
	param(
		[Parameter(Mandatory = $true)]
		[string]$FilePath,

		[Parameter()]
		[string[]]$ArgumentList = @()
	)

	& $FilePath @ArgumentList
	if ($LASTEXITCODE -ne 0) {
		throw "Command failed ($LASTEXITCODE): $FilePath $($ArgumentList -join ' ')"
	}
}

function Show-Help {
	@"
Modeler Windows helper

Usage:
	.\scripts\modeler.ps1 <command> [-Configuration Debug|Release] [-SystemDeps]

Commands:
  setup    Configure CMake
  build    Configure + build
  test     Configure + build + run tests
  run      Configure + build + run app
  docs     Generate Doxygen docs
  clean    Remove generated build directory
  help     Show this help

Examples:
  .\scripts\modeler.ps1 build
  .\scripts\modeler.ps1 test -Configuration Release
  .\scripts\modeler.ps1 run
"@
}

function Resolve-ToolchainFile {
	if ($SystemDeps) {
		return $null
	}

	$toolchain = Join-Path $RootDir "vcpkg\scripts\buildsystems\vcpkg.cmake"
	if (-not (Test-Path $toolchain)) {
		throw "Local vcpkg toolchain file not found at: $toolchain. Clone vcpkg into the project folder and run .\vcpkg\bootstrap-vcpkg.bat, or use -SystemDeps."
	}

	return $toolchain
}

function Invoke-Configure {
	$toolchain = Resolve-ToolchainFile
	$args = @(
		"-S", ".",
		"-B", "build",
		"-G", "Visual Studio 17 2022",
		"-A", "x64",
		"-DMODELER_BUILD_TESTS=ON",
		"-DCMAKE_BUILD_TYPE=$Configuration"
	)

	if ($toolchain) {
		$args += "-DCMAKE_TOOLCHAIN_FILE=$toolchain"
	}

	Push-Location $RootDir
	try {
		Invoke-External -FilePath "cmake" -ArgumentList $args
	}
	finally {
		Pop-Location
	}
}

switch ($Command) {
	"setup" {
		Invoke-Configure
	}
	"build" {
		Invoke-Configure
		Push-Location $RootDir
		try {
			Invoke-External -FilePath "cmake" -ArgumentList @("--build", "build", "--config", $Configuration, "--parallel")
		}
		finally {
			Pop-Location
		}
	}
	"test" {
		Invoke-Configure
		Push-Location $RootDir
		try {
			Invoke-External -FilePath "cmake" -ArgumentList @("--build", "build", "--config", $Configuration, "--parallel")
			Invoke-External -FilePath "ctest" -ArgumentList @("--test-dir", "build", "--build-config", $Configuration, "--output-on-failure")
		}
		finally {
			Pop-Location
		}
	}
	"run" {
		Invoke-Configure
		Push-Location $RootDir
		try {
			Invoke-External -FilePath "cmake" -ArgumentList @("--build", "build", "--config", $Configuration, "--parallel")

			$app = Join-Path $BuildDir "$Configuration\Modeler.exe"
			if (-not (Test-Path $app)) {
				$app = Join-Path $BuildDir "Modeler.exe"
			}
			if (-not (Test-Path $app)) {
				throw "Executable not found. Expected: $app"
			}

			Invoke-External -FilePath $app
		}
		finally {
			Pop-Location
		}
	}
	"docs" {
		Push-Location $RootDir
		try {
			Invoke-External -FilePath "doxygen" -ArgumentList @("docs/Doxyfile")
		}
		finally {
			Pop-Location
		}
	}
	"clean" {
		$generatedPaths = @(
			(Join-Path $BuildDir "Debug"),
			(Join-Path $BuildDir "Release"),
			(Join-Path $BuildDir "CMakeFiles"),
			(Join-Path $BuildDir "Testing"),
			(Join-Path $BuildDir "x64"),
			(Join-Path $BuildDir "cmake_install.cmake"),
			(Join-Path $BuildDir "CMakeCache.txt"),
			(Join-Path $BuildDir "build.ninja"),
			(Join-Path $BuildDir "rules.ninja"),
			(Join-Path $BuildDir "compile_commands.json"),
			(Join-Path $BuildDir "vcpkg-manifest-install.log")
		)

		foreach ($path in $generatedPaths) {
			if (Test-Path $path) {
				Remove-Item $path -Recurse -Force
			}
		}

		Write-Host "Removed generated build artifacts."
	}
	default {
		Show-Help
	}
}
