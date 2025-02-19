# tetris-cpp

управление:
  z - поворот против часовой стрелки
  x - движение влево
  c - движение вправо
  v - движение вниз
  q - закрыть программу

## инструкция по сборке

Нужен установленный Visual Studio Build Tools

Надо открыть пуск, найти открыть Developer Command Promt for VS 2022. 

1. Переходим в папку проекта
   ```bash
   cd /path/to/tetris-cpp
   ```
2. Прописываем следующие команды:
   ```bash
   cmake -DOpenCV_DIR="/path/to/opencv/build" .
   ```
   , где "/path/to/opencv/build" полный путь до папки, в которой лежит файл "opencvconfig.cmake". /path/to - путь до директории opencv.

   Следом пишем
   ```bash
   msbuild tetris.sln /property:Configuration=Release
   ```
   Далее переходим в папку Release и запускаем "tetris.exe". 
   ```bash
   cd Release
   tetris.exe
   ```
