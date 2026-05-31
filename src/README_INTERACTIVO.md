# SuperTux Interactive Patch - Option B MVP

Este parche agrega un sistema de webhook local para SuperTux usando un bridge Node.js.

## Archivos incluidos

- `src/supertux/interactive_bridge.hpp`
- `src/supertux/interactive_bridge.cpp`
- `bridge/supertux-bridge.js`

## Cambios manuales necesarios

### 1. Copia los archivos

Copia `src/supertux/interactive_bridge.hpp` y `src/supertux/interactive_bridge.cpp` dentro de tu repo:

`C:\Users\Administrator\Downloads\supertux-full\src\supertux\`

Copia la carpeta `bridge` dentro de la raíz:

`C:\Users\Administrator\Downloads\supertux-full\bridge\`

### 2. Edita `src/supertux/game_session.cpp`

Agrega este include cerca de los otros includes de `supertux/`:

```cpp
#include "supertux/interactive_bridge.hpp"
```

Busca la función:

```cpp
void
GameSession::update(float dt_sec, const Controller& controller)
{
```

Justo después de abrir la llave `{`, agrega:

```cpp
  if (m_currentsector != nullptr)
  {
    InteractiveBridge::process_pending_commands(*m_currentsector);
  }
```

Debe quedar parecido:

```cpp
void
GameSession::update(float dt_sec, const Controller& controller)
{
  if (m_currentsector != nullptr)
  {
    InteractiveBridge::process_pending_commands(*m_currentsector);
  }

  // Set active flag.
```

### 3. Commit y push

En GitHub Desktop verás cambios. Haz commit en la rama `interactive` y luego `Push origin`.

### 4. Compila en GitHub Actions

Ve a GitHub → Actions → Windows → Run workflow → branch `interactive`.

### 5. Prueba

Ejecuta el juego compilado y entra a un nivel.

En tu PC ejecuta:

```bash
cd C:\Users\Administrator\Downloads\supertux-full\bridge
node supertux-bridge.js
```

Pruebas:

```text
http://127.0.0.1:5720/health
http://127.0.0.1:5720/spawn?enemy=snowball&quantity=3&nickname=Fernando
http://127.0.0.1:5720/spawn?enemy=mriceblock&quantity=2&nickname=TikTokUser
http://127.0.0.1:5720/powerup?type=fire&quantity=1&nickname=Fernando
http://127.0.0.1:5720/powerup?type=star&quantity=1&nickname=Fernando
```

## IDs iniciales

Enemigos recomendados para primera prueba:

- `snowball`
- `mriceblock`
- `mrbomb`
- `jumpy`
- `spiky`
- `snail`
- `tarantula`

Powerups:

- `egg`
- `fire`
- `ice`
- `air`
- `earth`
- `star`
- `oneup`
