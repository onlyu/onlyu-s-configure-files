(setq cscope:map (make-sparse-keymap))
(define-key cscope:map "\C-cc" 'cscope-find-functions-calling-this-function)
(define-key cscope:map "\C-ct" 'cscope-find-this-text-string)
(define-key cscope:map "\C-cp" 'cscope-prev-symbol)
(define-key cscope:map "\C-cn" 'cscope-next-symbol)
(define-key cscope:map "\C-cP" 'cscope-prev-file)
(define-key cscope:map "\C-cN" 'cscope-next-file)
(define-key cscope:map "\C-cb" 'cscope-display-buffer)
(define-key cscope:map "\C-cu" 'cscope-pop-mark)

(setq cscope-display-cscope-buffer nil)

(defun mything-at-point (chars)
  (let* ((char-regx (concat "[" chars "]")))
    (save-excursion
      (buffer-substring-no-properties
       (progn
	 (if (not (looking-at char-regx))
	     (re-search-backward "\\w" nil t))
	 (skip-chars-backward chars)
	 (point))
       (progn
	 (skip-chars-forward chars)
	 (point)
	 )))
    ))
    
(defun mysymbol-at-point ()
  (let* ( (chars "A-Za-z0-9_") )
    (mything-at-point chars)
    ))
       

(defun myfile-at-point ()
  (let* ( (chars "-.,/A-Za-z0-9_~!@#$%&+=\\\\")  )
    (mything-at-point chars)
    ))

(defun mycfile-p (file)
  (or (string-match ".*\\.h$" file)
      (string-match ".*\\.c$" file)
      (string-match ".*\\.cc$" file)
      (string-match ".*\\.cpp$" file)))

(defun myprompt-for-symbol ()
  (let* ( (symbol (mysymbol-at-point))
	  (file (myfile-at-point))
	  (thing (if (mycfile-p file)
		     ;; find for file
		     file
		   ;; find symbol
		   symbol))
	  (prompt (if (or (not thing)
			  (string= thing ""))
		      "Find:"
		    (concat "Find (" thing "):")))
	  )
		  
    (setq sym (read-from-minibuffer prompt))
    
    (if (or (not sym)
	    (string= sym ""))
	thing
      sym)))

(defun myextract-symbol ()
  (let* ( (symbol (mysymbol-at-point))
	  (file (myfile-at-point)) )
    (if (mycfile-p file)
	       ;; find for file
	       file
	     ;; find symbol
	     symbol)
    ))

(defun mycscope-goto-prompt (symbol)
  "Find global definition or find file"
  (interactive (list
		(myprompt-for-symbol)
		))
  (if (mycfile-p symbol)

      (let ( (cscope-adjust nil) )	 ;; Disable fuzzy matching.
	(setq cscope-symbol symbol)
	(setq cscope-display-cscope-buffer nil)
	(cscope-call (format "Finding file: %s" symbol)
		     (list "-7" symbol) nil 'cscope-process-filter
		     'cscope-process-sentinel)
	)

    (let ( (cscope-adjust t) )	 ;; Use fuzzy matching.
      (setq cscope-symbol symbol)
      (setq cscope-display-cscope-buffer nil)
      (cscope-call (format "Finding global definition: %s" symbol)
		   (list "-1" symbol) nil 'cscope-process-filter
		   'cscope-process-sentinel)
      )
    ))

(defun mycscope-goto ()
  "Find global definition or find file no prompt"
  (interactive)
  (mycscope-goto-prompt (myextract-symbol)))

(defun mycscope-list-prompt (symbol)
  "List symbol or files include file"
  (interactive (list (myprompt-for-symbol) ))
  (if (mycfile-p symbol)

      (let ( (cscope-adjust nil) )	 ;; Disable fuzzy matching.
	(setq cscope-symbol symbol)
	(setq cscope-display-cscope-buffer t)
	(cscope-call (format "Finding files #including file: %s" symbol)
		     (list "-8" symbol) nil 'cscope-process-filter
		     'cscope-process-sentinel)
	)

    (let ( (cscope-adjust t) )	 ;; Use fuzzy matching.
      (setq cscope-symbol symbol)
      (setq cscope-display-cscope-buffer t)
      (cscope-call (format "Finding symbol: %s" symbol)
		   (list "-0" symbol) nil 'cscope-process-filter
		   'cscope-process-sentinel
		   t)
      )
    ))

(defun mycscope-list ()
  "List symbol or files include file no prompt"
  (interactive)
  (mycscope-list-prompt (myextract-symbol)))

;; make things more simple
(define-key cscope:map "\C-cg" 'mycscope-goto)
(define-key cscope:map "\C-cG" 'mycscope-goto-prompt)
(define-key cscope:map "\C-cl" 'mycscope-list)
(define-key cscope:map "\C-cL" 'mycscope-list-prompt)

(require 'xcscope)

(provide 'onlyu-xcscope)