function clone() {
    let container = document.getElementById('clones');
    container.insertAdjacentHTML('beforeend',
        `
        <button class="clone" onclick="clone()">clone</button>
        
        `
    );

}