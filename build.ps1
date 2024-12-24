
$ImagesFolders = @(
"Images\L2"
)

foreach($folder in $ImagesFolders)
{
    Write-Host ("Cобираю файлы в $folder...");

    $texFiles = Get-ChildItem -Path $folder -Filter "*.tex" -Recurse

    foreach ($file in $texFiles) {
        Write-Host "   Компиляция файла: '$($file.FullName)'"
        
        & xelatex -synctex=1 -interaction=nonstopmode --shell-escape -8bit -output-directory="$($file.DirectoryName)" "$($file.FullName)"

    }
}

