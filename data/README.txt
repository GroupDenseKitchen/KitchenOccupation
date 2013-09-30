För att köra CaviarGUI-labeler är det viktigt att en viss mappstruktur existerar. 

Din .jpg-sekvens måste ligga i en mapp med valfritt namn $NAME. I denna mapp skall en mapp som heter "JPEGS" finns. I denna mapp ligger din sekvens. Dessa filer inleds med samma namn $NAME som din huvudmapp och följs sedan av antingen tre eller fyra indexsiffror. Indexeringen skall börja på 001/0001. Det får inte finnas mellanslag mellan sekvensnamnet och indexering. 

Ex: $PWD\seq\JPEGS\seq001.jpg

"xmlcaviargui" måste ligga i samma mapp som mappen $NAME.

1 - Starta komandotolken och navigera till denna mapp
2 - kör: "java -mx128M -ms64M xmlcaviargui/CAVIARGUI"
	Anledningen till -mx128M och -ms64M är oklar, men måste vara där enligt Caviars manual.
3 - Tryck på "New JPG Seq"
	- Du frågas om det handlar om hundratals eller tusentals bilder för att programmet skall vet om indexeringen sker med tre eller fyra 		siffror. 
	- Ange $NAME som sequence name.
4 - Labela! Denna process beskrivs väl i http://homepages.inf.ed.ac.uk/rbf/CAVIARDATA1/xmlcaviargui/manual/manual.pdf


För att konvertera från .avi till .jpg kan till exempel "Free Video to JPG Converter" användas och för att fixa korrekta filnamn kan "Bulk Rename Utility" användas.