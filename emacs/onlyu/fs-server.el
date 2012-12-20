;; 封神服务器端相关脚本
(require 'onlyu-util)

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
				      (concat fs-server-dir "engine/update_file")
				      (list file-name)))
	  (set-process-filter update-process 'fs-update-filter)
	  (process-kill-without-query update-process)
	  (setq update-process nil))

      (message "can not update this file!"))
    ))

(defun puppy-reload ()
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


(defun project-root-helper (dir)
  (if (string= dir "")
      nil
    (if (file-exists-p (concat dir "/.eproject"))
	dir
      (project-root-helper (parent-dir dir)))))

(defun project-root ()
  (project-root-helper (parent-dir (buffer-file-name))))

(defun fs-walk-all-file (action)
  (walk-path fs-server-dir action))

(defun walk-project (action)
  (walk-path (project-root) action))

(defun suffixs-regexp (list)
  (if (null (cdr list))
      (concat "\\." (car list))
    (concat "\\." (car list) "\\|" (suffixs-regexp (cdr list)))))

(defvar src-regexp (suffixs-regexp '("c" "cc" "cpp" "h")))

(defvar g_old nil)
(defvar g_new nil)
(defun replace-in-c-visitor (old new)
  (setq g_old old)
  (setq g_new new)
  (lambda (filename)
    (if (and (string-match src-regexp filename) (file-exists-p filename))
	(with-temp-buffer
	  (insert-file-contents-literally filename)
	  (goto-char (point-min))
	  (if (re-search-forward g_old nil t 1)
	      (progn
		(goto-char (point-min))
		(replace-regexp g_old g_new)
		(write-region (point-min) (point-max) filename)))
	  t)
      t)))

(defun fs-rename (old new)
  (interactive (list ((lambda ()
			(let* ((thing (current-thing))
  			       (prompt (if (not thing) "rename:" (concat "rename (" thing "):"))))
			 (setq sym (read-from-minibuffer prompt))			 
			 (if (or (not sym) (string= sym "")) thing sym))))

  		     ((lambda()
  		       (let ((prompt "rename to:"))
  			 (setq sym (read-from-minibuffer prompt))
  			 sym)))))

  (walk-project (replace-in-c-visitor (concat "\\([^_0-9a-zA-Z]\\)" old "\\([^_0-9a-zA-Z]\\)")
					  (concat "\\1" new "\\2"))))

(defun current-thing ()
  (let* ((symbol (mysymbol-at-point))
	 (file (myfile-at-point))
	 (thing (if (mycfile-p file) file symbol)))
    (if (or (not thing) (string= thing "")) nil thing)))

(defun current-thing-regxp ()
  (let ((thing (current-thing)))
    (if thing (concat "\\([^_0-9a-zA-Z]\\)" thing "\\([^_0-9a-zA-Z]\\)") nil)))  

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

(add-hook 'lpc-mode-hook (lambda ()
			   (define-key pike-mode-map "\C-cj" 'fs-update)))

(global-set-key "\C-cr" 'fs-rename)
(provide 'fs-server)
