$cpp_files = Get-ChildItem -Filter "*.cpp" -Recurse

foreach ($file in $cpp_files)
{
    Set-Location $file.Directory

    cl $file.Name | Out-Null

    if ($LastExitCode -ne 0){
            Write-Host "$file Не собрался!" -ForegroundColor Red
            break
    }
    else {
        Write-Host "$file собран!" -ForegroundColor Green
    }

    Invoke-Expression  -Command "./$($file.BaseName) | out-null "
}

Write-Host "" 
Write-Host "Готово!" -ForegroundColor Green