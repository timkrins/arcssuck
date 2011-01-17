@cd "NC testing files"
@for %%i in (*converted.txt) do @del %%i
@for %%i in (*.NC) do (@..\arcssuck.exe %%i|@ping localhost -n 1 -w 1 > nul|@ren converted.txt %%i.converted.txt)
@ping localhost -n 2 -w 1000 > nul