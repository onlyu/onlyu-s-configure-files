(require 'flymake)

;; overide this function
(defun flymake-get-file-name-mode-and-masks (file-name)
  "Return the corresponding entry from `flymake-allowed-file-name-masks'."
  (if (file-writable-p file-name)
    (progn
      (unless (stringp file-name)
	(error "Invalid file-name"))
      (let ((fnm flymake-allowed-file-name-masks)
	    (mode-and-masks  nil))
	(while (and (not mode-and-masks) fnm)
	  (if (string-match (car (car fnm)) file-name)
	      (setq mode-and-masks (cdr (car fnm))))
	  (setq fnm (cdr fnm)))
	(flymake-log 3 "file %s, init=%s" file-name (car mode-and-masks))
	mode-and-masks))))

(defun flymake-get-make-cmdline (source base-dir)
  (list "gmake"
	(list "-s"
	      "-C"
	      base-dir
	      (concat "CHK_SOURCES=" source)
	      "SYNTAX_CHECK_MODE=1"
	      "check-syntax")))

(setq flymake-allowed-file-name-masks
      (cons '(".+(\\.c|\\.cc|\\.cpp)$"
	      flymake-simple-make-init
	      flymake-simple-cleanup
	      flymake-get-real-file-name)
	    flymake-allowed-file-name-masks))

(defun of-flymake-error ()
  (interactive)
  (let* ((line-no             (flymake-current-line-no))
	 (line-err-info-list  (nth 0 (flymake-find-err-info flymake-err-info line-no)))
	 (menu-data           (flymake-make-err-menu-data line-no line-err-info-list))
	 (choice              nil))
    (if menu-data
	(flymake-display-current-error)
      (flymake-goto-next-error-disp))))

(defun flymake-display-current-error ()
  "Display errors/warnings under cursor."
  (interactive)
  (let ((ovs (overlays-in (point) (1+ (point)))))
    (catch 'found
      (dolist (ov ovs)
        (when (flymake-overlay-p ov)
          (message (overlay-get ov 'help-echo))
          (throw 'found t))))))

(defun flymake-goto-next-error-disp ()
  "Go to next error in err ring, then display error/warning."
  (interactive)
  (flymake-goto-next-error)
  (flymake-display-current-error))

(defun flymake-goto-prev-error-disp ()
  "Go to previous error in err ring, then display error/warning."
  (interactive)
  (flymake-goto-prev-error)
  (flymake-display-current-error))

;; lpc error
;; module/base64_flymake.c line 4: Missing type for global variable declaration before the end of line
(setq flymake-err-line-patterns
      (cons '("\\(.*\\.[ch]\\) line \\([0-9]+\\): Warning: \\(.*\\)"
	      1 2 nil 3)
	    (cons '("\\(.*\\.[ch]\\) line \\([0-9]+\\): \\(.*\\)"
		    1 2 nil 3)
		  flymake-err-line-patterns)))

;; master file also use relative directory
(defun flymake-master-make-init (get-incl-dirs-f master-file-masks include-regexp)
  "Create make command line for a source file checked via master file compilation."
  (let* ((make-args nil)
	 (temp-master-file-name (flymake-init-create-temp-source-and-master-buffer-copy
                                 get-incl-dirs-f 'flymake-create-temp-inplace
				 master-file-masks include-regexp)))
    (when temp-master-file-name
      (let* ((buildfile-dir (flymake-init-find-buildfile-dir temp-master-file-name "Makefile")))
	(if  buildfile-dir
	    (setq make-args (flymake-get-syntax-check-program-args
			     temp-master-file-name buildfile-dir t t 'flymake-get-make-cmdline)))))
    make-args))

;; flymake-simple-make-init

(defun of-c-mode-common-hook ()
  (define-key c-mode-map "\C-ce" 'of-flymake-error)
  (define-key pike-mode-map "\C-ce" 'of-flymake-error)
  (define-key c++-mode-map "\C-ce" 'of-flymake-error))
  
;; use \C-ce for flymake
(setq flymake-log-level 3)
(setq flymake-gui-warnings-enabled nil)
(add-hook 'find-file-hooks 'flymake-find-file-hook)
(add-hook 'c-mode-common-hook 'of-c-mode-common-hook)

(provide 'onlyu-flymake)
