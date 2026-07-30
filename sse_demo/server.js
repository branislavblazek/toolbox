// Jednoduchý SSE server – posiela aktuálny čas v nepravidelných intervaloch (1–10 s).
// Spustenie: node server.js
const http = require("node:http");

const PORT = 3000;

function timestamp() {
  return new Date().toLocaleTimeString("sk-SK");
}

function handleSse(req, res) {
  res.writeHead(200, {
    "Content-Type": "text/event-stream",
    "Cache-Control": "no-cache",
    Connection: "keep-alive",
    // dovolí pripojenie aj keď je index.html otvorený priamo zo súboru (file://)
    "Access-Control-Allow-Origin": "*",
  });

  // po výpadku spojenia sa klient pokúsi znova pripojiť po 3 sekundách
  res.write("retry: 3000\n\n");

  console.log(`[${timestamp()}] client connected`);

  let timer;

  const scheduleNext = () => {
    const delayMs = 1000 + Math.floor(Math.random() * 9000); // 1–10 s

    timer = setTimeout(() => {
      res.write("data: update\n\n");
      console.log(`[${timestamp()}] update notification sent`);

      scheduleNext();
    }, delayMs);
  };
  scheduleNext();

  req.on("close", () => {
    clearTimeout(timer);
    console.log(`[${timestamp()}] client disconnected`);
  });
}

function handleUpdate(req, res) {
  // CORS preflight – prehliadač si pred requestom s Authorization hlavičkou
  // najprv overí, či ju server dovolí
  if (req.method === "OPTIONS") {
    res.writeHead(204, {
      "Access-Control-Allow-Origin": "*",
      "Access-Control-Allow-Headers": "Authorization",
    });
    res.end();
    return;
  }

  if (req.headers.authorization !== "sse-test") {
    res.writeHead(401, {
      "Content-Type": "text/plain; charset=utf-8",
      "Access-Control-Allow-Origin": "*",
    });
    res.end("Neautorizované");
    console.log(`[${timestamp()}] /updates rejected – invalid auth`);
    return;
  }

  res.writeHead(200, {
    "Content-Type": "text/plain; charset=utf-8",
    "Cache-Control": "no-cache",
    "Access-Control-Allow-Origin": "*",
  });
  res.end(timestamp());
  console.log(`[${timestamp()}] /updates fetched`);
}

const server = http.createServer((req, res) => {
  const { pathname } = new URL(req.url, `http://${req.headers.host}`);

  if (pathname === "/events") {
    handleSse(req, res);
  } else if (pathname === "/updates") {
    handleUpdate(req, res);
  } else {
    res.writeHead(404).end("Nenájdené");
  }
});

server.listen(PORT, () => {
  console.log(`Server beží na http://localhost:${PORT}`);
});
