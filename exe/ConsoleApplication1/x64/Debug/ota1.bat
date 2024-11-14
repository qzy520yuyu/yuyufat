:: bin文件名
@set src_filename=A01-3_V100

:: 需要命名成的文件名
@set dest_filename=update

:: .bin文件所在目录
@set src_path=objects

:: 命名后的文件放置的目标路径
@set dest_path=.\binary



@copy %src_path%\%src_filename%.bin %dest_path%\%dest_filename%.bin
@echo generated: %dest_filename%.bin

:: 文件末尾加入crc32校验，传入3个参数
call ConsoleApplication1 %dest_path%\%dest_filename%.bin %1 %2
