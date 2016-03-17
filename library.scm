(comment _________________________________________________________ )
(comment VERY USEFUL HELPERS )
(define comment 
  (lambda args (quote ())))
(define out
  (lambda args (print args)))
(define dumpln 
  (lambda args (print args)))
(define nil (quote ()))
(define length
  (lambda (list)
    (if (null? list)
	0
	(+ 1 (length (cdr list))))))

(comment _________________________________________________________ )
(comment ASSIGNMENT FUNCTIONS )
(define > (lambda (x y) (< y x)))
(define >= (lambda (x y) (not (< x y))))
(define <= (lambda (x y) (not (< y x))))
(define = (lambda (x y) (if (< x y) 0 (not (< y x)))))
(define abs (lambda (x) (if (< x 0) (- 0 x) x)))
(define factorial 
  (lambda (x) 
    (if (< x 2) 
	1 
	(* x (factorial (- x 1))))))
(define for-each
  (lambda (func list) 
    (func (car list)) 
    (if (not (null? (cdr list))) 
	(for-each func (cdr list)) 
	(quote ()))))
(define first (lambda args (car args)))
(define square (lambda (x) (print (* x x))))
(define swap2 
  (lambda (list)
    (cons (car (cdr list)) (cons (car list) (quote ())))))
(define neg (lambda (x) (< x 0)))
(define list-tail 
  (lambda (list k)
    (if (not (intp k))
	(print (quote INT_REQUIRED_ERROR))
	(if (< (length list) k)
	    (print (quote OUT_OF_BOUND_ERROR))
	    (if (< k 2)
		list
		(list-tail (cdr list) (- k 1)))))))
(define list-ref
  (lambda (list k)
    (car (list-tail list k))))
(define append
  (lambda (list1 list2)
    (if (nullp list1)
	list2
	(cons (car list1) (append (cdr list1) list2)))))
(define reverse
  (lambda (list)
    (if (nullp (cdr list))
        list
	(append (reverse (cdr list)) (cons (car list) (quote ()))))))
(define equal?
  (lambda (l1 l2)
    (if (list? l1)
	(if (not (list? l2))
	    0
	    (if (not (= (length l1) (length l2)))
		0
		(if (null? l1)
		    1
		    (if (list? (car l1))
			(if (list? (car l2))
			    (equal? (cdr l1) (cdr l2))
			    0)
			(if (= (car l1) (car l2))
			    (equal? (cdr l1) (cdr l2))
			    0)))))
	(= l1 l2))))

(define assoc
  (lambda (key dict)
    (if (nullp dict)
	0
	(if (= key (car (car dict)))
	    (car dict)
	    (assoc key (cdr dict))))))

(define append2car
  (lambda (elem list)
    (cons (append (cons elem (quote ()))
		  (car list))
	  (cdr list))))
(define append2cdr
  (lambda (elem list)
    (cons (car list)
	  (cons (append (cons elem (quote ()))
			(car (cdr list)))
		(quote ())))))
(define list-partition 
  (lambda (proc list)
    (if (nullp list)
	(quote (()()))
	(if (proc (car list))
	    (append2car (car list) (list-partition proc (cdr list)))
	    (append2cdr (car list) (list-partition proc (cdr list)))))))

(define list-same
  (lambda (list)
    (if (not (listp list))
	(print (quote NOT_A_LIST_ERROR))
	(if (< (length list) 2)
	    0
	    (if (nullp (cdr list))
		1
		(if (= (car list) (car (cdr list)))
		    (list-same (cdr list))
		    0))))))

(define list-partition-pivot
  (lambda (proc list)
    (if (nullp list)
	(quote (()()))
	(if (proc pivot (car list))
	    (append2car (car list) (list-partition-pivot proc (cdr list)))
	    (append2cdr (car list) (list-partition-pivot proc (cdr list)))))))
(define remove-pivot
  (lambda (pivot list)
    (if (nullp list)
	(quote ())
	(if (= pivot (car list))
	    (remove-pivot pivot (cdr list))
	    (cons (car list) (remove-pivot pivot (cdr list)))))))
(define clone-pivot
  (lambda (pivot num)
    (if (< num 1)
	(quote ())
	(cons pivot (clone-pivot pivot (- num 1))))))
(define count-pivot
  (lambda (pivot list)
    (if (nullp list)
	0
	(if (= pivot (car list))
	    (+ 1 (count-pivot pivot (cdr list)))
	    (count-pivot pivot (cdr list))))))
(define list-sort
  (lambda (proc list)
    (if (not (nullp list))
	(define pivot (car list)))
    (if (nullp list)
	list
	(if (< (length list) 2)
	    list
	    (append 
	     (append 
	      (list-sort 
	       proc 
	       (car (cdr (list-partition-pivot proc (remove-pivot pivot list)))))
	      (clone-pivot pivot (count-pivot pivot list)))
	     (list-sort 
	      proc 
	      (car (list-partition-pivot proc (remove-pivot pivot list)))))))))

(define even?
  (lambda (n)
    (if (< n 0)
	(even? (* n -1))
	(if (= n 0)
	    1
	    (if (= n 1)
		0
		(even? (- n 2)))))))
(define odd?
  (lambda (n)
    (not (even? n))))



(comment _________________________________________________________ )
(comment START GAME)

(comment _________________________________________________________ )
(comment SETTINGS )
(comment Note: dimensions have to be odd)
(define f_height 17)
(define f_width  37)
(define wall_symbol (quote #))
(define space_symbol (quote _))

(comment _________________________________________________________ )
(comment GENERATE GAME FIELD DATA STRUCTURE )
(define create-col
  (lambda (width)
    (if (= width 0)
	(quote ())
	(cons space_symbol (create-col (- width 1))))))
(define gen-field
  (lambda (width height)
    (if (= height 0)
	(quote ())
	(cons (create-col width) (gen-field width (- height 1)))))) 
(define create-field
  (lambda ()
    (gen-field f_width f_height)))
(comment _________________________________________________________ )
(define print-field
  (lambda (field)
    (print (car field))
    (if (nullp (cdr field))
	(quote ())
	(print-field (cdr field)))))

(comment _________________________________________________________ )
(comment PSEUDO RANDOM ACCESS ARRAY USING STRING HACK )
(comment Note: have to convert list to string first using (str) )
(define pos
  (lambda (x y)
    (+ 3 
       (* y (+ (* f_width 2) 2))
       (* 2 x))))
(define size
  (lambda () 
    (+ (* f_height 
	  (+ (* f_width 2) 2)) 
       1)))
(define set
  (lambda (field-str x y val)
    (appstr (appstr (substr field-str 
			    0 
			    (- (pos x y) 1))
		    val)
	    (substr field-str 
		    (pos x y) 
		    (- (size) (pos x y))))))
(define get
  (lambda (field-str x y)
    (substr field-str (- (pos x y) 1) 1)))

(comment _________________________________________________________ )
(comment GENERATE SMALLER ELEMENTS SUCH AS DOORS AND WALLS )
(comment Note: walls are always created on even positions, wheras
               doors on odd)
(define add-vdoor
  (lambda (field-str minY maxY x)
    (set field-str x (+ (* (/ (rand minY maxY) 2) 2) 1) space_symbol)))
(define add-hdoor
  (lambda (field-str minX maxX y)
    (set field-str (+ (* (/ (rand minX maxX) 2) 2) 1) y space_symbol)))

(define gen-hwall
  (lambda (field-str minX maxX y)
    (if (= minX maxX)
	(set field-str maxX y wall_symbol)
	(set (gen-hwall field-str minX (- maxX 1) y)
	     maxX
	     y
	     wall_symbol))))
(define gen-vwall
  (lambda (field-str minY maxY x)
    (if (= minY maxY)
	(set field-str x maxY wall_symbol)
	(set (gen-vwall field-str minY (- maxY 1) x)
	     x
	     maxY
	     wall_symbol))))

(comment _________________________________________________________ )
(comment GENERATE MAZE )
(comment Note: Using recursive devision algorithm to create the Maze)
(define gen-walls
  (lambda (field-str h minX maxX minY maxY)
    (comment print (print-field (parse field-str)))
    (comment out ____________________________________________________)
    (if (not (< (- maxY minY) 2))
	(define y (* (/ (rand minY maxY) 2) 2)))
    (if (not (< (- maxX minX) 2))
	(define x (* (/ (rand minX maxX) 2) 2)))
    (if h
	(if (< (- maxX minX) 2)
	    field-str
	    (gen-walls (gen-walls 
			(add-hdoor (gen-hwall field-str minX maxX y) minX maxX y) 
			(not h) minX maxX minY (- y 1))
		       (not h) minX maxX (+ y 1) maxY))
	(if (< (- maxY minY) 2)
	    field-str
	    (gen-walls (gen-walls 
			(add-vdoor (gen-vwall field-str minY maxY x) minY maxY x)
			(not h) minX (- x 1) minY maxY)
		       (not h) (+ x 1) maxX minY maxY)))))
(define add-frame
  (lambda (field-str)
    (gen-vwall (gen-vwall (gen-hwall (gen-hwall field-str 0 (- f_width 1) 0) 
				     0 (- f_width 1) (- f_height 1))
			  0 (- f_height 1) 0)
	       0 (- f_height 1) (- f_width 1))))
(define add-entrances
  (lambda (field-str)
    (set (set field-str 0 1 (quote _))
	 (- f_width 1) (- f_height 2) (quote _))))
(define gen-labyrinth
  (lambda (field-str)
    (add-entrances (add-frame (gen-walls field-str 0 1 (- f_width 2) 1 (- f_height 2))))))
(comment _________________________________________________________ )

(comment _________________________________________________________ )
(comment OLD CODE )
(comment Note: This is really slow shit, but kept for performance comparisons)
(define set-col
  (lambda (row w x y val)
    (if (= w 0)
	(quote ())
	(cons (if (= y 0)
		  (if (= x 0)
		      val
		      (car row))
		      (car row))
	      (set-col (cdr row) (- w 1) (- x 1) y val)))))
(define set-square
  (lambda (field w h x y val)
    (if (= h 0)
	(quote ())
	(cons (set-col (car field) w x y val) 
	      (set-square (cdr field) w (- h 1) x (- y 1) val)))))
(define add-list-vertical-wall
  (lambda (field x height)
    (if (= height 0)
	field
	(set-square (add-list-vertical-wall field x (- height 1))
		    f_width f_height x (- height 1) 0))))

(comment _________________________________________________________ )
(comment PERFORMANCE EXAMPLES )
(define example-strarr 
  (lambda () 
    (print-field (parse (gen-vwall 
			 (gen-vwall (str (create-field)) 0 (- f_height 1) 1)
			 0 (- f_height 1) 3)))))
(define example-list 
  (lambda () 
    (print-field (add-list-vertical-wall
		  (add-list-vertical-wall (create-field) 1 f_height)
		  3 f_height))))
(define example-performance
  (lambda ()
    (out START PERFORMANCE COMPARISON)
    (out Array using my fast 'pseudo random access' array)
    (example-strarr)
    (out)
    (out Array using slow cons list)
    (example-list)))
    
(comment _________________________________________________________ )
(comment LABYRINTH EXAMPLES )
(define example-labyrinth
  (lambda ()
    (print-field (parse (gen-labyrinth (str (create-field)))))))


(comment _________________________________________________________ )
(comment START UP )
(example-labyrinth)
