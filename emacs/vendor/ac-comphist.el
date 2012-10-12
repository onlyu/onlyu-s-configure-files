;; by Jr.White
;; auto complete history

;;;; Intelligent completion history

(defvar ac-comphist nil
  "Database of completion history.")

(defsubst ac-comphist-make-tab ()
  (make-hash-table :test 'equal))

(defsubst ac-comphist-tab (db)
  (nth 0 db))

(defsubst ac-comphist-cache (db)
  (nth 1 db))

(defun ac-comphist-make (&optional tab)
  (list (or tab (ac-comphist-make-tab)) (make-hash-table :test 'equal :weakness t)))

(defun ac-comphist-get (db string &optional create)
  (let* ((tab (ac-comphist-tab db))
         (index (gethash string tab)))
    (when (and create (null index))
      (setq index (make-vector (length string) 0))
      (puthash string index tab))
    index))

(defun ac-comphist-add (db string prefix)
  (setq prefix (min prefix (1- (length string))))
  (when (<= 0 prefix)
    (setq string (substring-no-properties string))
    (let ((stat (ac-comphist-get db string t)))
      (incf (aref stat prefix))
      (remhash string (ac-comphist-cache db)))))

(defun ac-comphist-score (db string prefix)
  (setq prefix (min prefix (1- (length string))))
  (if (<= 0 prefix)
      (let ((cache (gethash string (ac-comphist-cache db))))
        (or (and cache (aref cache prefix))
            (let ((stat (ac-comphist-get db string))
                  (score 0.0))
              (when stat
                (loop for p from 0 below (length string)
                      ;; sigmoid function
                      with a = 5
                      with d = (/ 6.0 a)
                      for x = (- d (abs (- prefix p)))
                      for r = (/ 1.0 (1+ (exp (* (- a) x))))
                      do
                      (incf score (* (aref stat p) r))))
              ;; Weight by distance
              (incf score (max 0.0 (- 0.3 (/ (- (length string) prefix) 100.0))))
              (unless cache
                (setq cache (make-vector (length string) nil))
                (puthash string cache (ac-comphist-cache db)))
              (aset cache prefix score)
              score)))
    0.0))

(defun ac-comphist-sort (db collection prefix &optional threshold)
  (let (result
        (n 0)
        (total 0)
        (cur 0))
    (setq result (mapcar (lambda (a)
                           (when (and cur threshold)
                             (if (>= cur (* total threshold))
                                 (setq cur nil)
                               (incf n)
                               (incf cur (cdr a))))
                           (car a))
                         (sort (mapcar (lambda (string)
                                         (let ((score (ac-comphist-score db string prefix)))
                                           (incf total score)
                                           (cons string score)))
                                       collection)
                               (lambda (a b) (< (cdr b) (cdr a))))))
    (if threshold
        (cons n result)
      result)))

(defun ac-comphist-serialize (db)
  (let (alist)
    (maphash (lambda (k v)
               (push (cons k v) alist))
             (ac-comphist-tab db))
    (list alist)))

(defun ac-comphist-deserialize (sexp)
  (condition-case nil
      (ac-comphist-make (let ((tab (ac-comphist-make-tab)))
                          (mapc (lambda (cons)
                                  (puthash (car cons) (cdr cons) tab))
                                (nth 0 sexp))
                          tab))
    (error (message "Invalid comphist db.") nil)))

(defun ac-comphist-init ()
  (ac-comphist-load)
  (add-hook 'kill-emacs-hook 'ac-comphist-save))

(defun ac-comphist-load ()
  (interactive)
  (let ((db (if (file-exists-p ac-comphist-file)
                (ignore-errors
                  (with-temp-buffer
                    (insert-file-contents ac-comphist-file)
                    (goto-char (point-min))
                    (ac-comphist-deserialize (read (current-buffer))))))))
    (setq ac-comphist (or db (ac-comphist-make)))))

(defun ac-comphist-save ()
  (interactive)
  (require 'pp)
  (ignore-errors
    (with-temp-buffer
      (pp (ac-comphist-serialize ac-comphist) (current-buffer))
      (write-region (point-min) (point-max) ac-comphist-file))))


(provide 'ac-comphist)
 
 ;;;; Auto completion internals
 
