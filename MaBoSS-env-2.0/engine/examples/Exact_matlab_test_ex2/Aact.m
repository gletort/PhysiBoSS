function A=Aact(RuB,RdB,RuC,RdC,T)
A=[];   
A(1,:)=[-RuB-2*T RdC+T T 0];
A(2,:)=[T -RdC-2*T 0 RdB+T];
A(3,:)=[RuB+T 0 -RuC-2*T T];
A(4,:)=[0 T RuC+T -RdB-2*T];
end
