@echo off
echo ============================
echo   GIT AUTO UPDATE SCRIPT
echo ============================

echo Enter commit message:
set /p msg="> "

git add .
git commit -m "%msg%"
git push

echo.
echo --- Update Complete! ---
pause
Fixed .gitignore file name
git add .gitignore
git commit -m "Fixed .gitignore file name"
git push    
echo.
echo --- .gitignore Fix Complete! ---