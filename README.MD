it is prototype version<br>
plz write me if you handle problems<br>
*in my pc it is also with problems but... lets check together:<br>

this app works with svg's like a with text files<br>
it is building and testing through GNOME builder<br>

testing steps:<br>
-clone repository<br>

test with test lib(99% must work):<br>
-copy files(pictures) from /libtest(in source of prototype) to /Pictures<br>
-build and run app:<br>
1)click search to view svg files in /Pictures<br>
2)click to element in search view to choose element for editing and for add it<br>
3)click to name(left panel) for choosing element<br>
4)edit element with control(buttons & scales)<br>
-look to /Pictures/output for look to result<br>

test with lib(1% must work - in my pc not working, looks strange plz try it on your pc):<br>
-copy files(pictures) from library(https://gitlab.gnome.org/bazylevnik0/icon-generator/-/tree/main/library) to /Pictures<br>
-go through same steps like a with test lib.<br>

*if errors:<br>
-delete /Pictures/output folder<br> 
-create issue with attachment result file(/Pictures/output/temp.svg)<br>
-run app again<br>

<img src="https://bazylevnik0.github.io/out/generator.png"></img>

