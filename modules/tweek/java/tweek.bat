set TWEEK_CLASSPATH=%TWEEK_BASE_DIR%/bin/jdom.jar;%TWEEK_BASE_DIR%/bin/xerces.jar;%TWEEK_BASE_DIR%/bin/Tweek.jar;%TWEEK_BASE_DIR%/bin/TweekBeans.jar;%TWEEK_BASE_DIR%/bin/TweekEvents.jar;%TWEEK_BASE_DIR%/bin/TweekNet.jar;%TWEEK_BASE_DIR%/bin/TweekBeanDelivery.jar;%TWEEK_BASE_DIR%/bin/TweekServices.jar;%TWEEK_BASE_DIR%/bin/kunststoff-mod.jar;%TWEEK_BASE_DIR%/bin/metouia.jar

java -DTWEEK_BASE_DIR=%TWEEK_BASE_DIR% -Djava.security.policy=%TWEEK_BASE_DIR%/bin/java.security.policy.txt -cp %TWEEK_CLASSPATH% org.vrjuggler.tweek.Tweek %1 %2 %3 %4 %5 %6 %7 %8 %9
