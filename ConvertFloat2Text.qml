    Text {
        id: text

        Component.onCompleted: {
            var i, k = 12.12312312312313;
            console.time('test ~~');
            for (i = 0; i < 100000; i++) {
                text.text = ~~k;
            }
            console.timeEnd('test ~~');

            console.time('test | 0');
            for (i = 0; i < 100000; i++) {
                text.text = k | 0;
            }
            console.timeEnd('test | 0');
        }
    }
    
// Unlike `https://jsperf.com/truncating-decimals` shows the fastest was `~~`, ` | 0` is the fastest way in the QML
// test ~~: 466ms
// test | 0: 33ms
