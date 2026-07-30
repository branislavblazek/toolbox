# SSE demo

## 1. zapni server

`node server.js`

## 2. otvor stránku

`open index.html`

## Ako to funguje

SSE stream (`/events`) slúži len ako notifikačný kanál – neprenáša žiadne dáta.
Keď príde notifikácia, klient si aktuálne dáta vyžiada samostatným requestom
na `/updates`, autentifikovaným hlavičkou `Authorization: sse-test`.

```mermaid
sequenceDiagram
    participant B as Prehliadač (index.html)
    participant S as Server (server.js, port 3000)

    B->>S: GET /events (EventSource)
    S-->>B: 200 text/event-stream, retry: 3000
    Note over B: status: online

    loop každých 1–10 s (náhodne)
        S-->>B: SSE správa "data: update"
        Note over B: onmessage – zaloguje<br/>"update notification received"

        B->>S: OPTIONS /updates (CORS preflight)
        S-->>B: 204 Allow-Headers: Authorization

        B->>S: GET /updates<br/>Authorization: sse-test

        alt hlavička = "sse-test"
            S-->>B: 200 aktuálny čas
            Note over B: zobrazí čas a pridá ho do logu
        else nesprávna / chýbajúca hlavička
            S-->>B: 401 Neautorizované
            Note over B: zobrazí "fetch failed"
        end
    end

    Note over B,S: pri výpadku spojenia sa EventSource<br/>automaticky znova pripojí po 3 s
```
