F�r att k�ra CaviarGUI-labeler �r det viktigt att en viss mappstruktur existerar. 

Din .jpg-sekvens m�ste ligga i en mapp med valfritt namn $NAME. I denna mapp skall en mapp som heter "JPEGS" finns. I denna mapp ligger din sekvens. Dessa filer inleds med samma namn $NAME som din huvudmapp och f�ljs sedan av antingen tre eller fyra indexsiffror. Indexeringen skall b�rja p� 001/0001. Det f�r inte finnas mellanslag mellan sekvensnamnet och indexering. 

Ex: $PWD\seq\JPEGS\seq001.jpg

"xmlcaviargui" m�ste ligga i samma mapp som mappen $NAME.

1 - Starta komandotolken och navigera till denna mapp
2 - k�r: "java -mx128M -ms64M xmlcaviargui/CAVIARGUI"
	Anledningen till -mx128M och -ms64M �r oklar, men m�ste vara d�r enligt Caviars manual.
3 - Tryck p� "New JPG Seq"
	- Du fr�gas om det handlar om hundratals eller tusentals bilder f�r att programmet skall vet om indexeringen sker med tre eller fyra 		siffror. 
	- Ange $NAME som sequence name.
4 - Labela! Denna process beskrivs v�l i http://homepages.inf.ed.ac.uk/rbf/CAVIARDATA1/xmlcaviargui/manual/manual.pdf


F�r att konvertera fr�n .avi till .jpg kan till exempel "Free Video to JPG Converter" anv�ndas och f�r att fixa korrekta filnamn kan "Bulk Rename Utility" anv�ndas.