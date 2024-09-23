astyle --style=kr --indent=tab --pad-paren --pad-header --pad-oper "*.c" "*.h" --recursive
find . -name "*.orig" -type f -delete


#find . -name "*.c" -type f -exec sed -i '1,2!{/PVS-Studio Static Code Analyzer for C, C++ and C#/d;}' {} +
