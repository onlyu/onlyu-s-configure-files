;; 封神服务器端相关脚本

(defcustom fs-server-dir "/home/onlyu/trunk/"
  "fs server trunk directory"
  :group 'fs
  :type 'sexp)

(defvar fs-logic-dir (concat fs-server-dir "logic" "/")
  "fs server logic directory")

(defvar fs-engine-dir (concat fs-server-dir "engine" "/")
  "fs server engine directory")

(defun fs-update-filter (process str)
  (message str)
  (kill-buffer "*fs*"))

(defun fs-update ()
  (interactive)
  (let* ((file-name (buffer-file-name)))
    (if (string-match fs-logic-dir file-name)
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

(add-hook 'c-mode-common-hook (lambda ()
				(define-key c-mode-map "\C-cj" 'fs-update)))
(add-hook 'pike-mode-hook (lambda ()
			    (define-key pike-mode-map "\C-cj" 'fs-update)))

(provide 'fs-server)