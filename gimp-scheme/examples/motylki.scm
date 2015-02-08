( define ( motylki image warstwa n ) 
		 ( let* ( 
				  ( wys ( car ( gimp-image-height image ) ) )
				  ( szer ( car ( gimp-image-width image ) ) )
				  ( i 0 )
				  )
				( while ( < i n )
						( let* ( 
								 ( tmp ( car ( gimp-edit-paste warstwa 100 ) ) )
								 ( wystmp ( car ( gimp-drawable-height tmp ) ) )
								 ( szertmp ( car ( gimp-drawable-width tmp ) ) )
								 )
							   ( gimp-layer-set-offsets tmp ( rand ( - szer szertmp ) ) ( rand ( - wys wystmp ) ) )
							   ( gimp-drawable-transform-rotate tmp ( / ( rand 180 ) 6.28 ) 1 0 0 0 0 0 3 0 )
							   ( print i )
							   ( set! i ( + i 1 ) )
							   ;( gimp-layer-translate tmp ( - 0 ( rand wys ) ) ( - 0 ( rand szer ) ) )
							   )
						)
				( gimp-image-merge-visible-layers image 1 )
				)
		 )

(script-fu-register "motylki"
					_"<Image>/Script-Fu/kubek2k/Motylki..."
					"Wrzuca n zaznaczen..."
					"Jakub Janczak"
					"Jakub Janczak"
					"January 2005"
					""
					SF-IMAGE "Image" 0
					SF-DRAWABLE "Drawable" 0
					SF-VALUE      _"Size" "10")



