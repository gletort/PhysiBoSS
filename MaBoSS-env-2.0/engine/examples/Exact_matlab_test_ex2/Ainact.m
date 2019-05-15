function A=Ainact(RuB,RdB,RuC,RdC,T)
A=[];
A(1,:)=[ -2*T T+RdC T+RdB 0];
A(2,:)=[T -2*T-RdC 0 T+RdB];
A(3,:)=[T 0 -2*T-RdB-RuC T];
A(4,:)=[0 T T+RuC -2*T-RdB];
end
