;;
;;  Skrypt do wymazywania co n-tej kolumny
;; 	Jakub Janczak
;; 
;;
(define (script-fu-wymaz-co-n img drawable n)
  (gimp-undo-push-group-start img)
  (let* (
  			(szer (car (gimp-drawable-width drawable)))
  			(wys (car (gimp-drawable-height drawable)))
			( x 0 )
		)
		(
  			(while (< x szer )
        	 (gimp-rect-select img x 0 1 wys REPLACE FALSE 0)
    	     (gimp-edit-clear drawable)
         	 (set! x (+ x n))
        	)
		)
	)
  	(gimp-selection-none img)
  	(gimp-undo-push-group-end img)
  	(gimp-displays-flush)
  )

(script-fu-register "script-fu-wymaz-co-n"
                    _"<Image>/Script-Fu/kubek2k/Wymaz co n-ta kolumne..."
                    "Wymazuje co n-ta kolumne"
                    "Jakub Janczak"
                    "GNU GPL"
                    "9 sty 2005"
                    "RGB* GRAY* INDEXED*"
                    SF-IMAGE "Image" 0
                    SF-DRAWABLE "Drawable" 0
                    SF-VALUE "Co n-ty pixel" "3"
)
