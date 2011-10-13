;; look-up english word

(require 'get-url-content)
(require 'json)

(defgroup onlyu-dict nil
  "look-up word with google, thank you google!!"
    :group 'convenience)
(defcustom dict-url "www.google.com/dictionary/json?callback=display&q=%s&sl=en&tl=zh-CN&restrict=pr,de&client=te"
  ""
  :group 'onlyu-dict
  :type 'sexp)

(defun onlyu-find-word (word)
  "find the word in google dict, return it's means"
  (interactive "sword: ")
  (progn
    (setq url (format dict-url word))
    (setq str (get-url-content url))
    (with-output-to-temp-buffer "onlyu-dict-mode"
      (print (replace-regexp-in-string "^.*display(\\(.*\\))" "\\1" str)))))
      ;;(print (json-read-from-string str)))))

(provide 'onlyu-dict)
