;; 封神服务器端相关脚本

(defcustom fs-server-dir "/home/onlyu/trunk/"
  "fs server trunk directory"
  :group 'fs
  :type 'sexp)

(defun fs-logic-dir ()
  (concat fs-server-dir "logic" "/"))

(defun fs-engine-dir ()
  (concat fs-server-dir "engine" "/"))

(defun fs-update-filter (process str)
  (message str)
  (kill-buffer "*fs*"))

(defun fs-source-file-p (file)
  (string-match fs-server-dir file))

(defun fs-update ()
  (interactive)
  (let* ((file-name (buffer-file-name)))
    (if (string-match (fs-logic-dir) file-name)
	(progn 
	  (setq file-name (replace-match "" nil nil file-name))
	  ;; create process and run
	  (setq update-process (apply 'start-process "update-file" 
				      "*fs*" 
				      (concat fs-server-dir "shell/update_file")
				      (list file-name)))
	  (set-process-filter update-process 'fs-update-filter)
	  (process-kill-without-query update-process)
	  (setq update-process nil))

      (message "can not update this file!"))
    ))

(defun fs-reload-all ()
  (interactive)
  (mapcar (lambda (buffer)
	    (let ((file (buffer-file-name buffer)))
	      (if (and (stringp file) (file-regular-p file) (fs-source-file-p file) (file-exists-p file))
		  (if (verify-visited-file-modtime buffer)
		      (if (buffer-modified-p buffer)
			  (with-current-buffer buffer
			    (revert-buffer t nil))
			(with-current-buffer buffer
			  (revert-buffer t t)))))))
	  (buffer-list)))

(defun fs-new-syntax ()
  "change to new syntax"
  (interactive)
  (message "success"))

(defun walk-path (dir action)
  "walk DIR executing ACTION with (dir file)"
  (cond ((file-directory-p dir)
	   (or (char-equal ?/ (aref dir(1- (length dir))))
	       (setq dir (file-name-as-directory dir)))
	   (let ((lst (directory-files dir nil nil t))
		 fullname file)
	     (while lst
	       (setq file (car lst))
	       (setq lst (cdr lst))
	       (cond ((member file '("." ".." ".svn" ".git")))
		     (t
		      (and (funcall action (concat dir file))
			   (setq fullname (concat dir file))
			   (file-directory-p fullname)
			   (walk-path fullname action )))))))
	  (t
	   (funcall action dir))))


(defun fs-walk-all-file (action)
  (walk-path fs-server-dir action))

(defvar g-rep-old nil)
(defvar g-rep-new nil)

(defun replace-in-c-visitor (old new)
  (setq g-rep-new new)
  (setq g-rep-old old)
  (lambda (filename)
    (if (string-match "\\.[ch]$" filename)
	;(message (format "cat %s | sed 's/%s/%s/g' > %s" filename g-rep-old g-rep-new filename))
	;(start-process "replace" "*fs*" "sh" (format "cat %s | sed 's/%s/%s/g' > %s" filename g-rep-old g-rep-new filename))
	(with-temp-buffer
	  (insert-file-contents-literally filename)
	  (goto-char (point-min))
	  (if (re-search-forward g-rep-old nil t 1)
	      (progn
		(goto-char (point-min))
		(replace-regexp g-rep-old g-rep-new)
		(write-region (point-min) (point-max) filename)))
	  t)
      t)))

(defun fs-rename (old new)
  (interactive (list ((lambda ()
  		       (let* ( (symbol (mysymbol-at-point))
  			       (file (myfile-at-point))
  			       (thing (if (mycfile-p file)
  					  ;; find for file
  					  file
  					;; find symbol
  					symbol))
  			       (prompt (if (or (not thing)
  					       (string= thing ""))
  					   "rename:"
  					 (concat "rename (" thing "):")))
  			       )
			 (setq sym (read-from-minibuffer prompt))
			 
			 (if (or (not sym)
				 (string= sym ""))
			     thing
			   sym))))

  		     ((lambda()
  		       (let ((prompt "rename to:"))
  			 (setq sym (read-from-minibuffer prompt))
  			 sym)))))

  (fs-walk-all-file (replace-in-c-visitor (concat "\\([^_0-0a-zA-Z]\\)" old "\\([^_0-0a-zA-Z]\\)") 
					  (concat "\\1" new "\\2")))
  (fs-reload-all)
  (revert-buffer (current-buffer) t t))

(defun walk-path-visitor (file)
  "Called by walk-path for each file found"
  (if (string-match "\\.[ch]$" file)
      (progn
	(message file)
	t)
    t))

(add-hook 'c-mode-common-hook (lambda ()
				(define-key c-mode-map "\C-cj" 'fs-update)))

(add-hook 'pike-mode-hook (lambda ()
			    (define-key pike-mode-map "\C-cj" 'fs-update)))

(global-set-key "\C-cr" 'fs-rename)
(provide 'fs-server)