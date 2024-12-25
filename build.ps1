
#Сборка изображений

$ImagesFolders = @(
#"Images\L2",
"Images\L3"
)

foreach($folder in $ImagesFolders)
{
    Write-Host ("Cобираю файлы в $folder...");

    $texFiles = Get-ChildItem -Path $folder -Filter "*.tex" -Recurse

    foreach ($file in $texFiles) {
        Write-Host "   Компиляция файла: '$($file.FullName)'"
        
        $null = xelatex -synctex=1 -interaction=nonstopmode --shell-escape -8bit -output-directory="$($file.DirectoryName)" "$($file.FullName)" 

        if ($LastExitCode -ne 0)
        {
            Write-Host "      !!Ошибка: '$($file.FullName)'. См. лог сборки" -ForegroundColor red
        }

    }
}

