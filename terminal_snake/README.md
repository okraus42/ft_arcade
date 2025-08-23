server just runs
client
- offline practice
- online
-- Join server
-- Host LAN
-- Join LAN

need to get username from environment and first 6 characters from hostname to locate the computer.

Spectator mode 

Scoring (snake length * food)
Losing points for hitting yourself (- snake length * number of continuous hits)
Getting points for other snakes hitting you (snake length * number of continuous hits for combo)

Game speed increasing over time

Server
- 5 minute wait to let people register
- then only spectators can join

server requests data
server receives and processes data (otherwise keeps old values)
server sends updated state
server requests data

client receives state
client draws
client receives request
client sends data
client receives state


PLAN
1) game 
-  solo on server
-  1v1 on server
-  group on server
2) tournament system
3) scoreboard
4) stats
5) game chat

Friday

Saturday
- handshake (11:00)
-- server sends secret (42)
-- client checks (42)
-- client sends secret (42, random uint_8)
-- server verifies client (got 42 + secret)
-- server sends secret (secret + secret xor)
-- client verifies secret (secret + secret xor)
-- timeout on inactive conenctions 15 seconds (later)
- register user (14:00)
-- client sends username + hostname start
-- server sends game data 
- client waiting window (16:00)
-- countdown 60 seconds
- client game window (before player can move) (17:00)
// refuse if the same name exists and is active -> spectator status
// unsigned long union with char 8 - check names
// coalitions naboo, alderaan, mandalore, tatooine, other
- solo game

Sunday
- multiplayer
- parsing coalition names