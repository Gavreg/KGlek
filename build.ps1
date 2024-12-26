
#Сборка изображений

$PSVersionTable.PSVersion

$ImagesFolders = @(
    "Images\L2"
    "Images\L3"
    "Images\L4"
)

$texfiles = @();
foreach ($folder in $ImagesFolders) {
    $texFiles += Get-ChildItem -Path $folder -Filter "*.tex" -Recurse
}


Write-Host ""
Write-Host ("Файлы на сбор:");
Write-Host ""
$texFiles | ForEach-Object {$_.FullName}

Write-Host ""
Write-Host ("Cобираю файлы:");
Write-Host ""

$texFiles | ForEach-Object -Parallel { 
    $null = xelatex -synctex=1 -interaction=nonstopmode -halt-on-error --shell-escape -8bit -output-directory="$($_.DirectoryName)" "$($_.FullName)" 
    if ($LastExitCode -ne 0) {
        Write-Host "    !!Ошибка: '$($_.FullName)'. См. лог сборки" -ForegroundColor red
    }
    else {
        Write-Host "    Успешно!: '$($_.FullName)'." -ForegroundColor green
    }
} -AsJob  |  Wait-Job | Receive-Job


Write-Host ""
Write-Host ("Завершено!");
Write-Host ""
