;;
;; Grid-overlay superimposes a semi-transparent grid 
;; giving a tiled effect.
;;
(define (script-fu-grid-overlay img drawable color size)
  (define (draw-line drawable startx starty endx endy)
    (let ((line (cons-array 4 'double)))
      (aset line 0 startx)
      (aset line 1 starty)
      (aset line 2 endx)
      (aset line 3 endy)
      (gimp-pencil drawable 4 line)
      ))
  (gimp-undo-push-group-start img)
  (let* ((w (car (gimp-drawable-width drawable)))
         (h (car (gimp-drawable-height drawable)))
         (grid-layer (car (gimp-layer-new img w h 
                                          RGB-IMAGE 
                                          "grid" 
                                          100 
                                          NORMAL-MODE)))
         (sx 0)
         (sy 0)
         )
    (gimp-layer-add-alpha grid-layer)

    (gimp-image-add-layer img grid-layer -1)
    
    (gimp-rect-select img 0 0 w h REPLACE FALSE 0)
    (gimp-edit-clear grid-layer)
    
    (gimp-palette-set-foreground color)
    (gimp-brushes-set-brush "pixel (1x1 square)")

    (while (< sy h)
           (set! sy (+ sy size))
           (draw-line grid-layer 0 sy w sy)
           )
    (while (< sx w)
           (set! sx (+ sx size))
           (draw-line grid-layer sx 0 sx h)
           )
    
    (gimp-layer-set-opacity grid-layer 25)
    
    (gimp-selection-none img)
    (gimp-undo-push-group-end img)
    (gimp-displays-flush)
    )
)

(script-fu-register "script-fu-grid-overlay"
                    _"<Image>/Script-Fu/kubek2k/Dodaj przezroczysta siatke..."
                    "Adds Grid Overlay to image"
                    "Walter Higgins"
                    "Walter Higgins"
                    "Nov 2004"
                    "RGB* GRAY* INDEXED*"
                    SF-IMAGE "Image" 0
                    SF-DRAWABLE "Drawable" 0
                    SF-COLOR      _"Color"         '(255 255 255)
                    SF-VALUE      _"Size" "10")
