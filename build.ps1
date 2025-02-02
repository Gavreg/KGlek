#Функция построения tex файлов
function Build-Files {
    param(
        $files
    )

    Write-Host ("Cобираю файлы:");
    Write-Host ""

    $jobs = @()

    foreach ($file in $files) {
        $jobs += (Start-Job -ScriptBlock {

                Set-Location "$($input.DirectoryName)" 

                $null = xelatex -synctex=1 -interaction=nonstopmode -halt-on-error --shell-escape -8bit -output-directory="$($using:file.DirectoryName)" "$($using:file.FullName)" 
                if ($LastExitCode -ne 0) {
                    Write-Host "    !!Ошибка: '$(
                $using:file.FullName)'. См. лог сборки" -ForegroundColor red
                }
                else {
                    Write-Host "    Успешно!: '$(
                $using:file.FullName)'." -ForegroundColor green
                }
            } -InputObject $file ) 
    }

    $total = $jobs.Count

    While ($jobs.Count -ne 0) {
        $job = ($jobs | Wait-Job -Any)
        Write-Host "$($total - $jobs.Count + 1)/$total " -NoNewline
        $job | Receive-Job
        $jobs = ($jobs | Where-Object { $_ -ne $job })
    }


    Write-Host ""
    Write-Host ("Завершено!");
}


#Сборка изображений
#папки изображений, можно отключать, коментируя ненужное  #`ой
$ImagesFolders = @(
    "Images\L1"    
    "Images\L2"
    "Images\L3"
    "Images\L4"
    "Images\L5"
    "Images\L6"
    "Images\L7"
    "Images\L8"
    "Images\L9"
    "Images\L10"
    "Images\L11"
    "Images\L12"
    "Images\L13"
)

$lectionsTexFiles = Get-ChildItem -Filter "L*.tex" 

$contentFile = Get-ChildItem content.tex

$imagesTexFiles = @()
foreach ($folder in $ImagesFolders) {
    $imagesTexFiles += Get-ChildItem -Path $folder -Filter "*.tex" -Recurse
}


#Собираем изображения
Build-Files -files $imagesTexFiles

#Собираем лекции
Build-Files -files $lectionsTexFiles

#Еще раз собираем лекции
Build-Files -files $lectionsTexFiles

#Собираем оглавление
Build-Files -files $contentFile

