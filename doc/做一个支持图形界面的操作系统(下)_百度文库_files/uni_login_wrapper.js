function isSingleInstanceProd(s){var n=["mn","ma","im_hi"];return("|"+n.join("|")+"|").indexOf("|"+s+"|")>-1}function isLoginInstance(s){var s=s||"login";return"login"==s}function saveInitInstance(s){window._pass_popinit_instance=s}function getInitInstance(){return window._pass_popinit_instance}var passport=passport||window.passport||{};passport._modulePool=passport._modulePool||{},passport._define=passport._define||function(s,n){passport._modulePool[s]=n&&n()},passport._getModule=passport._getModule||function(s){return passport._modulePool[s]},passport.pop=passport.pop||{},passport.pop.insertScript=passport.pop.insertScript||function(s,n){var i=document,a=i.createElement("SCRIPT");a.type="text/javascript",a.charset="UTF-8",a.readyState?a.onreadystatechange=function(){("loaded"==a.readyState||"complete"==a.readyState)&&(a.onreadystatechange=null,n&&n())}:a.onload=function(){n&&n()},a.src=s,i.getElementsByTagName("head")[0].appendChild(a)},passport.pop.init=passport.pop.init||function(s){var n={"http:":"http://passport.bdimg.com","https:":"https://ss0.bdstatic.com/5LMZfyabBhJ3otebn9fN2DJv"};if(isSingleInstanceProd(s.apiOpt.product)&&isLoginInstance(s.type)&&getInitInstance())return getInitInstance();if(s&&"https"==s.protocol)var i="https:";else var i=window.location?window.location.protocol.toLowerCase():document.location.protocol.toLowerCase();var a=["pp","mn","wk","cmovie","translate","baidugushitong","ik","exp","waimai","jn","im","do","yuedu","hao123","tb","netdisk","developer","newdev","image_eco","zbsc","bpit_hcm","defensor"],e=s&&s.apiOpt&&s.apiOpt.product||"",t=("|"+a.join("|")+"|").indexOf("|"+e+"|")>-1,p="/passApi/js/uni_login_a769f42.js",c="/passApi/js/uni_login_tangram_c82a860.js",o="/passApi/css/uni_login_new_a9b11fc.css";t&&(p="/passApi/js/uni_loginv4_d42190d.js",c="/passApi/js/uni_loginv4_tangram_2862f9e.js",o="/passApi/css/uni_loginv4_1c50fab.css");var _={uni_login:p,uni_login_tangram:c,uni_loginPad:"/passApi/js/uni_loginPad_c46ece1.js",uni_loginPad_tangram:"/passApi/js/uni_loginPad_tangram_49dd4b3.js",uni_smsloginEn:"/passApi/js/uni_smsloginEn_63d59f4.js",uni_smsloginEn_tangram:"/passApi/js/uni_smsloginEn_tangram_db94d4f.js",uni_loginWap:"/passApi/js/uni_loginWap_17f3847.js",uni_loginWap_tangram:"/passApi/js/uni_loginWap_17f3847.js",uni_accConnect:"/passApi/js/uni_accConnect_75bbd25.js",uni_accConnect_tangram:"/passApi/js/uni_accConnect_tangram_46541bf.js",uni_accRealName:"/passApi/js/uni_accRealName_d66f5c0.js",uni_accRealName_tangram:"/passApi/js/uni_accRealName_tangram_5c446eb.js",uni_checkPhone:"/passApi/js/uni_checkPhone_19e1fe4.js",uni_checkPhone_tangram:"/passApi/js/uni_checkPhone_tangram_f8d834b.js",uni_checkIDcard:"/passApi/js/uni_checkIDcard_abc5433.js",uni_checkIDcard_tangram:"/passApi/js/uni_checkIDcard_tangram_69ef53e.js",uni_accSetPwd:"/passApi/js/uni_accSetPwd_a6c18f4.js",uni_accSetPwd_tangram:"/passApi/js/uni_accSetPwd_tangram_b37293c.js",uni_IDCertify:"/passApi/js/uni_IDCertify_223c79c.js",uni_IDCertify_tangram:"/passApi/js/uni_IDCertify_tangram_432d69b.js",uni_travelComplete:"/passApi/js/uni_travelComplete_b566822.js",uni_travelComplete_tangram:"/passApi/js/uni_travelComplete_tangram_ac0b13f.js",uni_IDCertifyQrcode:"/passApi/js/uni_IDCertifyQrcode_6c16d38.js",uni_IDCertifyQrcode_tangram:"/passApi/js/uni_IDCertifyQrcode_tangram_c71713b.js",uni_loadingApi:"/passApi/js/uni_loadingApi_9fd229c.js",uni_loadingApi_tangram:"/passApi/js/uni_loadingApi_tangram_1e7ece7.js",uni_secondCardVerify:"/passApi/js/uni_secondCardVerify_7922c8c.js",uni_secondCardVerify_tangram:"/passApi/js/uni_secondCardVerify_tangram_3e82451.js",uni_multiBind:"/passApi/js/uni_multiBind_9c76660.js",uni_multiBind_tangram:"/passApi/js/uni_multiBind_tangram_b55cbd6.js",uni_multiUnbind:"/passApi/js/uni_multiUnbind_91f11fb.js",uni_multiUnbind_tangram:"/passApi/js/uni_multiUnbind_tangram_9538a65.js",uni_changeUser:"/passApi/js/uni_changeUser_f25ade6.js",uni_changeUser_tangram:"/passApi/js/uni_changeUser_tangram_0a53059.js",uni_loginMultichoice:"/passApi/js/uni_loginMultichoice_affc1cf.js",uni_loginMultichoice_tangram:"/passApi/js/uni_loginMultichoice_tangram_f2d1e77.js",uni_confirmWidget:"/passApi/js/uni_confirmWidget_fb4667e.js",uni_confirmWidget_tangram:"/passApi/js/uni_confirmWidget_tangram_dd48979.js"},r={login:o,loginWap:"/passApi/css/uni_loginWap_f57424a.css",smsloginEn:"/passApi/css/uni_smsloginEn_eef0a6a.css",accConnect:"/passApi/css/uni_accConnect_ab6dda9.css",accRealName:"/passApi/css/uni_accRealName_a224a37.css",secondCardVerify:"/passApi/css/uni_secondCardVerify_98a0d35.css",checkPhone:"/passApi/css/uni_checkPhone_cd7c7a0.css",checkIDcard:"/passApi/css/uni_checkIDcard_be79680.css",accSetPwd:"/passApi/css/uni_accSetPwd_29f7784.css",IDCertify:"/passApi/css/uni_IDCertify_36e091b.css",IDCertifyQrcode:"/passApi/css/uni_IDCertifyQrcode_1e8827b.css",loadingApi:"/passApi/css/uni_loadingApi_f8732c0.css",loginPad:"/passApi/css/uni_loginPad_af389a4.css",multiBind:"/passApi/css/uni_multiBind_e8d24e4.css",multiUnbind:"/passApi/css/uni_multiUnbind_21428a6.css",changeUser:"/passApi/css/uni_changeUser_c7ae7b4.css",loginMultichoice:"/passApi/css/uni_loginMultichoice_289d3a0.css",confirmWidget:"/passApi/css/uni_confirmWidget_073965e.css",uni_rebindGuide:"/passApi/css/uni_rebindGuide_347ecf2.css",travelComplete:"/passApi/css/uni_travelComplete_8a662dd.css"},u=n[i]||n["https:"],s=s||{};s.type=s.type||"login";var d,l=document,s=s||{},g=("_PassUni"+(new Date).getTime(),u+r[s.type]);s.cssUrlWrapper&&(g=cssUrlWrapper.join(t?"uni_loginv4.css":"uni_login.css")),d={show:function(){return d.loadPass(s.apiOpt),d.willShow=!0,d},setSubpro:function(n){return s.apiOpt&&(s.apiOpt.subpro=n),d},setMakeText:function(n){return s.apiOpt&&(s.apiOpt.makeText=n),d},loadPass:function(){var n=l.createElement("link");n.rel="stylesheet",n.type="text/css",n.href=g,n.disabled=!1,n.setAttribute("data-for","result"),l.getElementsByTagName("head")[0].appendChild(n),d.show=function(){return d.willShow=!0,d},s.plugCss&&(n=l.createElement("link"),n.rel="stylesheet",n.type="text/css",n.href=s.plugCss,n.disabled=!1,n.setAttribute("data-for","result"),l.getElementsByTagName("head")[0].appendChild(n)),d.passCallback(),delete d.loadPass},passCallback:function(){d.components.length>0?passport.pop.insertScript(d.components.shift(),d.passCallback):(passport.pop[s.type](s,d,function(){d.willShow&&d.show(),s&&s.onRender&&s.onRender()}),delete d.passCallback,delete d.components)},components:[]};var m="uni_"+s.type;return s.tangram&&(m+="_tangram"),s.apiOpt&&"ik"==s.apiOpt.product&&Math.random()<.3&&(d.components.push(u+"/passApi/js/uni/passhunt.js"),s.hunter=!0),d.components.push(u+_[m]),s.cache&&d.loadPass(s.apiOpt),s.id&&l.getElementById(s.id)&&(l.getElementById(s.id).onclick=function(){d.show()}),isSingleInstanceProd(s.apiOpt.product)&&isLoginInstance(s.type)&&saveInitInstance(d),d};