(setq gnus-select-method '(nntp "groups.google.com"))
(setq user-full-name "onlyu")
(setq user-mail-address "efiish@mail.com")
(setq gnus-default-subscribed-newsgroups
        '("gnu.emacs.help"
        "gnu.emacs.sources"))

;; set email reader 
;; (setq gnus-secondary-select-methods '((nnml ""))) 
(add-to-list 'gnus-secondary-select-methods '(nnimap "gmail"
                                  (nnimap-address "imap.gmail.com")
                                  (nnimap-server-port 993)
                                  (nnimap-stream ssl)))

(setq message-send-mail-function 'smtpmail-send-it
      smtpmail-starttls-credentials '(("smtp.gmail.com" 587 nil nil))
      smtpmail-auth-credentials '(("smtp.gmail.com" 587 "efiish@gmail.com" nil))
      smtpmail-default-smtp-server "smtp.gmail.com"
      smtpmail-smtp-server "smtp.gmail.com"
      smtpmail-smtp-service 587
      smtpmail-local-domain "while1.webfaction.com")

