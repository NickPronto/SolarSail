# SolarSail
Arduino code for sensor and actuator controls.

I’ve been developing a fun little side project: A lay flat, solar tracker for mobile/emergency/home use. Would love to get any feedback you had on the code or the sketches!!

Attached is my first stab at the code for the solar array and a reference to the mechanics. I met with a fabricator yesterday and they are down to help build prototypes! I just started learning Arduino a few days ago, so don’t judge the code too harshly :) It compiles at least!

The problem I’m trying to solve is most mobile applications for solar panel tracking 1) Don’t actually track the sun, they just tilt, and 2) can’t track over 45 degrees in BOTH directions. This design will allow the panel to locate and move to the most efficient angle and adjust to the suns movement, regardless of the vehicle's cardinal direction AND lay flat securely when not in use. The Arduino and mechanics should allow this to be tied to the vehicles ignition (or master switch) and have little to no necessary interaction with the user.

The lifting component of this is a single linear actuator and a lever arm that guides the center pivot point of the solar panel array up while either the left or right push/pull solenoids are engaged, acting as a hinge point. 

For example:

If the ignition switch is off, the photo sensors Left and Right will compare their outputs. If the Left is higher, there is more sun in that direction. The right solenoid hinge pin will release, while the Left remains engaged. The linear actuator will then push the center pivot point of the panel array straight up, causing the panel array to angle towards the sun. Once the sensors equalize, the panel should stop, and adjust (by lowering the linear actuator). Once the linear actuator has reached its lowest limit, the panel should be flat. As the sun continues to move, the code should swap the solenoid engagements, and begin to lift the center actuator again, repeating the process.

If at any point both the light sensors drop below a threshold (like at night), OR the ignition switch comes back on, the panels will reset to the lowest point and all the hinge solenoids will engage, locking all sides down to the vehicle.
