let count = 0;
function countUp() {
    let counter = document.getElementById('counter');
    count = count + 1;
    counter.innerText = count;
}
function countDown() {
    let counter = document.getElementById('counter');
    count = count - 1;
    counter.innerText = count;
}