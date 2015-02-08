( define ( hello-world image warstwa text font-size font )
		 ( let* ( ( nasztext ( car ( gimp-text-fontname image -1 
											( / ( car (  gimp-image-width image ) ) 2 )
											( / ( car ( gimp-image-height image ) ) 2 )
														  text
														  0 TRUE font-size 1
														  font ) )
				  ) 
				  ( kopia ( car (gimp-layer-copy nasztext 1 ) ) ) 
				 )
				  ( gimp-image-add-layer image kopia 0 )
				  ( plug-in-gauss 1 image kopia 3 3 0 )
				  ( gimp-layer-translate kopia 3 3  )
				  ( gimp-image-raise-layer image nasztext )
				  ( gimp-image-merge-down image nasztext 0 )
		 		 ( gimp-displays-flush )
		 )
)
			    

(script-fu-register "hello-world"
					_"<Image>/Script-Fu/kubek2k/Hello World..."
					"Wypisuje dowolny tekst i tworzy cien..."
					"Jakub Janczak"
					"Jakub Janczak"
					"January 2005"
					""
					SF-IMAGE "Image" 0
					SF-DRAWABLE "Drawable" 0
					SF-STRING      "Text" "Hello World!!!"
					SF-VALUE "Font-size" "14"
					SF-FONT "Fontname" "Arial" )



