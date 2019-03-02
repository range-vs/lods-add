# Lods Add

## Русское описание
Программа создана для работы с X-Ray SDK любой версии. Тестировалось на X-Ray SDK 0.7.
Представляет из себя программу для пакетного перемещения лодов из одной локации на другую со смещением позиции.

### Использование:
* Создайте любой *.bat - файл, и в него запишите команду запуска.
* Запустите *.bat - файл и дождитесь окончания операции

### Как это работает?
Если Вы соединяете локации, и у Вас нет желания расставлять все объекты и лоды(которые стоят в нулевых коррдинатах, что важно)
по новой, то Вы можете воспользоваться данным софтом. Вам требуется получить мировые координаты смещения вашего терреина в любом 3D - 
редакторе(тестировалось в Autodesk Maya). И далее запустить *bat - файл с записанными туда координатами.

### Описание команд:
* -translation "три вещественных числа через пробел",  дробная часть отделяется точкой. Означает позицию, куда перемещать входные секции

### Структура каталогов:
Если каталогов нет, то требуется создать рядом с программой.
* levels\in\ - здесь должен располагаться файл **scene_object.part**, из которого будут браться секции
* levels\modify\ - здесь должен распологаться файл **scene_object.part**, к которому будут добавляться секции
* levels\out\ - здесь создасться выходной файл **scene_object.part**, хранящий секции двух локаций

### Пример:
```
-translation 1837.168 -43.138 336.515
```

### Дополнительно:
Если Вы берёте координаты из редактора Autodesk Maya, то коорднату z **обязательно** следует инвертировать.


## Russian description
The program is designed to work with the X-Ray SDK of any version. Tested on X-Ray SDK 0.7.
It is a program for batch moving lodov from one location to another with offset position.

### Use:
* Create any * .bat file, and write the startup command in it.
* Run * .bat file and wait for the operation to finish

### How it works?
If you connect locations, and you have no desire to arrange all objects and Lods (which are in zero coordinates, which is important)
on the new, then you can use this software. You need to get the world coordinates of your terrain in any 3D -
editor (tested in Autodesk Maya). And then run the * bat - file with the coordinates recorded there.

### Description of commands:
* -translation "three real numbers separated by a space", the fractional part is separated by a period. Means the position where to move the input sections

### Directory structure:
If there are no directories, then you need to create next to the program.
* levels\in\ - here should be a file **scene_object.part**, from which sections will be taken
* levels\modify\ - the file **scene_object.part** should be placed here, to which sections will be added
* levels\out\ - here the output file **scene_object.part** is created, which stores sections of two locations

### Example:
```
-translation 1837.168 -43.138 336.515
```

### Optional:
If you take coordinates from an Autodesk Maya editor, then the z **coordinate must** be inverted.
