@echo off & setLocal EnableDelayedExpansion
set /a num=!random! %% 2

exit %num%
