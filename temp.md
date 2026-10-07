
1) Use case
Челикс открывает консольку и прописывает:
```bash
waterMark-tool </path/to/file.bmp> </path/to/watermark.bmp> <path/to/result.bmp>
```

2) Main logic steps
* Parse arguments
* Validate arguments
* Load file.bmp and watermark.bmp
* Call func to apply watermark on file.bmp
* Save result

3) BmpFile
- Конструкторы через: путь до файла, 
- Диструктор
- Методы: 
- - Гетеры:
* Получить цвет пикселя по координатам
- - Сетеры:
* Установить цвет пикселя по координатам
* Изменить путь до файла
- - Прочие:
* Сохранить изменения
