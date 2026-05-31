// SuperTux TikTok/Inkafinity bridge MVP
// Run: node supertux-bridge.js
// Endpoints:
//   http://127.0.0.1:5720/spawn?enemy=snowball&quantity=3&nickname=Fernando
//   http://127.0.0.1:5720/powerup?type=fire&quantity=1&nickname=Fernando

const http = require('http');
const fs = require('fs');
const os = require('os');
const path = require('path');

const PORT = Number(process.env.PORT || 5720);
const COMMAND_FILE = path.join(os.tmpdir(), 'supertux_interactive_commands.txt');

function cleanText(value, fallback = '') {
  return String(value ?? fallback).replace(/[;\r\n]/g, ' ').trim().slice(0, 64);
}

function cleanId(value, fallback) {
  return String(value ?? fallback).replace(/[^a-zA-Z0-9_-]/g, '').trim().slice(0, 48) || fallback;
}

function toInt(value, fallback = 1, min = 1, max = 25) {
  const n = Number.parseInt(value, 10);
  if (!Number.isFinite(n)) return fallback;
  return Math.max(min, Math.min(max, n));
}

function writeCommand(fields) {
  const line = Object.entries(fields).map(([k, v]) => `${k}=${v}`).join(';') + '\n';
  fs.appendFileSync(COMMAND_FILE, line, 'utf8');
}

function send(res, status, data) {
  res.writeHead(status, { 'Content-Type': 'application/json; charset=utf-8', 'Access-Control-Allow-Origin': '*' });
  res.end(JSON.stringify(data));
}

const server = http.createServer((req, res) => {
  try {
    const url = new URL(req.url, `http://127.0.0.1:${PORT}`);
    const nickname = cleanText(url.searchParams.get('nickname') || url.searchParams.get('userName') || url.searchParams.get('username'), 'viewer');
    const quantity = toInt(url.searchParams.get('quantity') || url.searchParams.get('count'), 1, 1, 25);

    if (url.pathname === '/spawn') {
      const enemy = cleanId(url.searchParams.get('enemy') || url.searchParams.get('id'), 'snowball');
      writeCommand({ action: 'spawn', enemy, quantity, nickname });
      return send(res, 200, { ok: true, action: 'spawn', enemy, quantity, nickname });
    }

    if (url.pathname === '/powerup') {
      const type = cleanId(url.searchParams.get('type') || url.searchParams.get('id'), 'fire');
      writeCommand({ action: 'powerup', type, quantity: Math.min(quantity, 10), nickname });
      return send(res, 200, { ok: true, action: 'powerup', type, quantity: Math.min(quantity, 10), nickname });
    }

    if (url.pathname === '/health') {
      return send(res, 200, { ok: true, commandFile: COMMAND_FILE });
    }

    send(res, 404, { ok: false, error: 'Use /spawn or /powerup' });
  } catch (err) {
    send(res, 500, { ok: false, error: String(err.message || err) });
  }
});

server.listen(PORT, '127.0.0.1', () => {
  console.log(`[SuperTux Bridge] Listening on http://127.0.0.1:${PORT}`);
  console.log(`[SuperTux Bridge] Command file: ${COMMAND_FILE}`);
});
