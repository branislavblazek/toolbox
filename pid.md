```bash

# spustenie servera v terminal
node server.js

# spustenie detached servera
nohup node server.js > server.log 2>&1 &
nohup node server.js > /dev/null 2>&1 & # zahod vystup
nohup node server.js &> /dev/null & # zahod vystup, kratsie

# kill vsetko na 3000
kill $(lsof -ti:3000)

# vypis co bezi na 3000 + command (LISTEN je ten proces, co vysiela)
ps -o command -p $(lsof -ti:3000)
ps -o command -p $(lsof -ti:3000 -sTCP:LISTEN)

```
