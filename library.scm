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
(define nil (quote ()))
(define comment 
  (lambda args (print args)))
(define for-each
  (lambda (func list) 
    (func (car list)) 
    (if (not (null? (cdr list))) 
	(for-each func (cdr list)) 
	(quote ()))))
(define length
  (lambda (list)
    (if (null? list)
	0
	(+ 1 (length (cdr list))))))
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
