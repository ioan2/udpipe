Example to start a server with two models

 ./udpipe_server --no_log  --no_preload_default 33333 frtok \
	frtok /data2/udpipe/ud-2.0/fr-ud-train.tokenizer.model UD1 \
	frtagdep /data2/udpipe/ud-2.0/UD_XPOS_NOISE-200-edims50-swap-tag1M-xpos/udpipe.model UD2



example to use both models with curl in a pipe

 curl -s -F "data=le chat a mangé. Elle dort maintenant." -Ftokenizer= -Fmodel=frtok http://yd-jeuh6401:33333/process \
	| jq -r ".result" \
	| curl -s -Fdata=@- -Ftagger= -Fparser= -Fmodel=frtagdep http://yd-jeuh6401:33333/process



