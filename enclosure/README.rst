====================================
arduino_current_controller enclosure
====================================

* Press eight #4 pem nuts and four #6 pem nuts into 0.25" thick base
  plate.

* Press two #6 pem nuts into 0.125" side plate.

* Attach (pcb + arduino) to 0.25" thick base plate.

* Connect a 1k resistor between IM483 P2:1 and P2:2. Connect a 1.6k
  resistor between P2:2 and P2:3.

* Attach IM483 to 0.25" thick base plate 6-32 x 1/2" socket head cap
  screws.

* Cut 4-pin 12" jumper wires 3" from connector. Separate wires back 1"
  and strip wires 1/4".

* Connect RED to IM483 P1:5, ORANGE to P1:4, YELLOW to P1:3, and
  GREEN to P1:2.

* Plug 4-pin connector to P2 on pcb.

* Set IM483 current resolution switch to OFF, OFF, OFF, ON, to set
  resolution to 256 microsteps/step.

* Cut UNSHIELDED 22AWG 2 COND to 3" and remove outer gray
  insulation. Solder to CONN JACK 2.5x5.5MM, RED to center, BLACK to
  terminal next to center terminal, and cut off the 3rd terminal. Cut
  wire insulation 1/8" on CONN JACK side and 1/4" on other side. Use
  3/32" x 1/2" heat shrink tubing.

* Screw CONN JACK 2.5x5.5MM into 0.125" side plate hole.

* Cut UNSHIELDED 22AWG 4 COND to 2" and remove outer gray
  insulation. BLACK: 2", GREEN: 1 7/8", WHITE: 1 3/4", RED: 1
  5/8". Cut wire insulation 1/4" on both sides. Solder to TERMINAL PB
  4CIRC 2.50" MOUNT, BLACK closest to PCB, then GREEN, then WHITE,
  then RED. With a BLUE marker, color WHITE until BLUE. Use 1/8" x
  3/4" heat shrink tubing.

* Screw TERMINAL PB 4CIRC into 0.125" side plate with two 6-32 x 3/8"
  socket head cap screws with a #6 washer on each.

* Connect TERMINAL PB 4CIRC BLACK to P2:5, GREEN to P2:6, BLUE to
  P2:7, and RED to P2:8.

* Connect CONN JACK BLACK to PCB P3:GND and CONN JACK RED to PCB
  P3:+24VDC.

* Cut another UNSHIELDED 22AWG 2 COND to 3" and remove
  insulation. Connect BLACK to PCB P3:GND and RED to PCB P3:+24VDC.

* Connect BLACK to IM483 P2:3 and RED to IM483 P2:4.

* Place colored tape on spring terminal tops, BLACK, GREEN, and
  BLUE. (the RED terminal is already RED).


Author: Peter Polidoro

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
