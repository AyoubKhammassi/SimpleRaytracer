make
if %errorlevel% neq 0 exit /b %errorlevel%
ECHO Build finised
forfiles /p "." /m "*.exe" /c "@file"
out.ppm