$rootDir = Join-Path $PSScriptRoot ".."

$includeRootDir = Join-Path $rootDir "include"
$sourceRootDir = Join-Path $rootDir "src"

Get-ChildItem -Path $includeRootDir -Include *.h -Recurse | ForEach-Object -Process {
    Push-Location $includeRootDir
        $relPath = Resolve-Path -Path $_.FullName -Relative
    Pop-Location
    $guard = (($relPath -replace '[\\.]','_') -replace '^_*(?<Content>.*)','${Content}').ToUpper()

    $current = (Get-Content $_.FullName -TotalCount 1) -replace '^#ifndef\s*',''

    if ($guard -ne $current) {
        (Get-Content $_.FullName) -replace $current,$guard | Set-Content $_.FullName
    }
}

Get-ChildItem -Path $sourceRootDir -Include *.h -Recurse | ForEach-Object -Process {
    Push-Location $sourceRootDir
        $relPath = Resolve-Path -Path $_.FullName -Relative
    Pop-Location
    $guard = (($relPath -replace '[\\.]','_') -replace '^_*(?<Content>.*)','${Content}').ToUpper()

    $current = (Get-Content $_.FullName -TotalCount 1) -replace '^#ifndef\s*',''

    if ($guard -ne $current) {
        (Get-Content $_.FullName) -replace $current,$guard | Set-Content $_.FullName
    }
}