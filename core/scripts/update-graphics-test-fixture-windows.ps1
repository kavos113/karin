$env:UPDATE_TEST_IMAGES=1
$env:TEST_PROFILE="windows"

Write-Host "Updating graphics test fixture for Windows"

Push-Location -Path (Join-Path $PSScriptRoot "..\..\build\windows-local-debug\core\test")

./karin_test.exe
Write-Host "Done updating graphics test fixture for Windows"
Pop-Location

Copy-Item -Path (Join-Path $PSScriptRoot "..\..\build\windows-local-debug\core\test\fixtures\test_expect_images\windows") -Destination (Join-Path $PSScriptRoot "..\test\fixtures\test_expect_images") -Recurse -Force


$env:TEST_PROFILE="windows-vulkan"

Write-Host "Updating graphics test fixture for Windows Vulkan"

Push-Location -Path (Join-Path $PSScriptRoot "..\..\build\windows-vulkan-local-debug\core\test")

./karin_test.exe

Write-Host "Done updating graphics test fixture for Windows Vulkan"
Pop-Location

Copy-Item -Path (Join-Path $PSScriptRoot "..\..\build\windows-vulkan-local-debug\core\test\fixtures\test_expect_images\windows-vulkan") -Destination (Join-Path $PSScriptRoot "..\test\fixtures\test_expect_images") -Recurse -Force
