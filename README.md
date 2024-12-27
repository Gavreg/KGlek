# Презентации к курсу лекций по Компьютерной графике
(2024)

09.03.01 Информатика и вычислительная техника

## Инструкция по сборке

Для сборки этих презентаций я применял следующее ПО:

- Tex Live 2024
- Visual Studio 2022
- Power Shell 7.5

Любое из этих средств может быть заменено аналогом, 
но скрипты с,роки написаны исходя из того, что у меня используется именно это ПО.

Структура:

- Лекции L*.tex и их оглавление content.tex.
- Изображения к лекциям в папке Images.
- Изображения в формате tex в папке Images. Нуждаются в предварительной сборке.
- Программы для генерации данных для некоторых изображений.

При сборке лекций создаются файлы *.gvr, из которых собирается файл с оглавлением лекций content.tex

Для сборки: 

0. Убедится в доступности XeLatex из командной строки Power Shell

```  xelatex -version ```

1. Разрешить выполнение сценариев в Power Shell, команда требует прав администратора

``` Set-ExecutionPolicy -ExecutionPolicy RemoteSigned ```

2. В Developer PowerShell for VS перейти в папку с лекциями и выполнить скрипт 
   build_cpp.ps1. Он скомпилирует программы и выполнить программы, генерирующие материалы для примеров

3. В Power Shell выполнить сценарий build.ps1, который соберет все изображения в папке Image,
2 раза все лекции, файл с содержанием лекций. У меня скрипт корректно отрабатывал только в 7м Power Shell.
Для частичной сборки курса можно комментировать участки скрипта, отключая ненужные директории и файлы.