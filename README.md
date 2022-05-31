# AproxPac-Man

Proiectul constă într-o parte software și una hardware în cazul soluției alese de mine(soluție aleasă din lipsă de timp în perioada deadline-ului de predare al proiectului). 

Partea hardware constă în conectarea unei matrici de led-uri(am avut la îndemână doar de 8x8) RGB adresabilă(LED de tip WS2812B) și a unui joystick la o plăcuță de dezvoltare Arduino UNO(Model care-mi va cauza inconveniențe pe viitor în implementare din cauza lipsei de memorie).
Partea software constă în mișcarea efectivă a unui Player și a unui sau mai multor Enemies prin matrice( fiind doar 64 de poziții este greu a face ceva foarte complex ) considerând totodată și boundarie-uri de tip pereți, și viteza de mișcare a unui Enemy)

Librării folosite pentru implementare :

 Adafruit_NeoPixel.h
Link-uri:
• Referințe - https://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html
• Sursă 1 - https://github.com/adafruit/Adafruit_NeoPixel
• Sursă 2 - https://adafruit.github.io/Adafruit_NeoPixel/html/_adafruit___neo_pixel_8h_source.html

 AxisJoistick.h
Link-uri :
• Sursă + referințe - https://github.com/YuriiSalimov/AxisJoystick


Detalii de implementare :

Implementarea este una destul de rudimentară în principiu din cauza lipsei de memorie a plăcii Arduino dar evident și a lipsei de timp în perioada aceasta.
// Notă : structura de bază a proiectului este luată de la un alt proiect pentru facultate la care nu am folosit un sistem de versionare, motiv pentru care multe modificări nu se văd îmn partea de history a repository-ului 
Se folosesc două clase principale Player și Enemy(care exind clasa Led - fiecare poziție a matricii este un led) care implementează metode de movement(și multe alte potențiale funcționalități) care se adaug unor hărți hard-coded care diferențiază passable terrain(0) de pereți(1) după care oferă culoarea respectivă fiecăruia în array-ul de culori al matricii de led-uri)
// Notă : înt-o versiune anterioară a codului, am încercat să face clase și pentru pereți și passable terrain dar a trebuit să renunț din lipsă de memorie.
În fucția de loop sunt verificate condiții pentru alegere hartă( jocul fiind aproximativ pe nivele, spre deosebire de pacman original) și setate flag-uri în funcție de progresul în joc
