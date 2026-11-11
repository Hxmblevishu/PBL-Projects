# SurakshaPath feature test sequence

Run the app from a temporary copy of `data/` so test actions do not alter the
project's real CSV files. For each case, use the numbered menu option, enter
values when prompted, and press Enter at the return-to-menu prompt. The cases
below were run in an isolated copy with the compiled Windows executable.

| Case | Feature | Test steps | Result |
|---|---|---|---|
| T01 | Menu and text input | Select a menu number and enter start/destination IDs. | **PASS** — numeric menu input and route prompts worked. UP/DOWN was not exercised in this run. |
| T02 | BFS route | Select 1; start ID 1, destination ID 3. | **PASS** — route and distance/risk summary appeared. |
| T03 | DFS route | Select 2; start ID 1, destination ID 3. | **PASS** — depth-first route appeared. |
| T04 | Dijkstra route | Select 3; start ID 1, destination ID 3. | **PASS** — route and safety-cost summary appeared. |
| T05 | Add location | Select 4; enter `Test Library`; connect to ID 1 with distance 4 km and risk 2. | **PASS** — saved as `20,Test Library` and the road saved. |
| T06 | Show map | Select 5. | **PASS** — map listed the new place and road. |
| T07 | Add situation risk | Select 6; report `Heavy rain test` at ID 1, severity 4. | **PASS** — incident saved and contributed to route risk. |
| T08 | Block road | Select 7; IDs 1 and 6; choose 1. Then select 5. | **PASS** — header count changed and map showed the road blocked. |
| T09 | Unblock road | Select 7; IDs 1 and 6; choose 2. Then select 5. | **PASS** — header count returned to zero and map showed the road open. |
| T10 | Compare routes | Select 8; start ID 1, destination ID 3. | **PASS** — BFS, DFS, and Dijkstra results all appeared. |
| T11 | Disaster submenu: add | Select 9, then 1; report `Road accident test` at ID 3, severity 5. | **PASS** — incident saved. Routing treats adjacent roads as closed. |
| T12 | Disaster submenu: view | Select 9, then 2. | **PASS** — incident records showed severity and active/closed status. |
| T13 | Disaster submenu: close | Select 9, then 3; close incident ID 3. View again. | **PASS** — incident 3 was marked CLOSED and persisted. |
| T14 | Disaster closure on map | Add an active severity-5 incident at ID 3, then select 5. | **FAIL** — map still labels roads touching ID 3 as OPEN; header still says BLOCKED 0. Routing does block those roads. |
| T15 | Disaster submenu: back | Select 9, then 4. | **PASS** — returned to the main route desk. |
| T16 | System information | Select 10. | **PASS** — algorithm notes, locations, and saved weather details appeared. |
| T17 | Exit | Select 11. | **PASS** — app exited without another prompt. |

## Results

The sequence was rerun on 2026-11-09. It exited with code 0 and no standard-
error output. Test data was kept in a copy; the project's original CSV files
were not modified. The only failed case was T14.

## Bugs found

1. **Disaster closures are missing from the map and blocked-road count.** A
   severity-5 active incident prevents route algorithms from using adjacent
   roads, but the map reads only the manual blocked flag and labels them OPEN.
   The dashboard also reports `BLOCKED 0`. This can mislead someone viewing the
   network after a disaster.
2. **The dashboard needs a wide terminal.** The two-column menu is 90 columns
   wide and wraps in an 80-column terminal. Use a terminal width of at least 90
   columns for the current layout.
